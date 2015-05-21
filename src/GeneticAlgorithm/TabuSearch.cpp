/**
 * @file TabuSearch.cpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since GeneticNet 1.0
 * @date 06/06/2012
 */

#include <iomanip>
#include <boost/random.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/concept_check.hpp>

#include "TabuSearch.hpp"
#include "GeneticOperators.hpp"

namespace geneticnet {
	namespace TS {
		boost::random::mt19937 rng;
		boost::random::uniform_int_distribution<> intrand;
		boost::random::uniform_real_distribution<> realrand;
		const char* setup_xml_name = "TabuSearchSetup";
	}
	
	TabuSearch::TabuSearch() {
		this->setup = new TS::Setup();
	}
	
	TabuSearch::TabuSearch(TS::Setup* tsSetup) : setup(tsSetup) {
		if (this->setup == 0) {
			std::cerr << "Error loading Tabu Search: NULL setup.\n";
			abort();
		}
	}
	
	TabuSearch::TabuSearch(boost::archive::xml_iarchive& input) {
		TS::Setup* setup = new TS::Setup();
		input >> boost::serialization::make_nvp<TS::Setup>(TS::setup_xml_name, (*setup));
		this->setup = setup;
	}
	
	TabuSearch::~TabuSearch() {
		delete this->setup;
	}
	
	void TabuSearch::save_setup_to_xml(boost::archive::xml_oarchive& output) {
		output << boost::serialization::make_nvp<TS::Setup>(TS::setup_xml_name, *(this->setup));
	}
	void TabuSearch::load_setup_from_xml(boost::archive::xml_iarchive& input) {
		input >> boost::serialization::make_nvp<TS::Setup>(TS::setup_xml_name, *(this->setup));
	}
	
	void TabuSearch::execute(ResultManager * results) {
		bool doResult = (results != NULL);
		loggerpp::log("Starting Tabu Search execution...", loggerpp::level::INFO, this);
		
		if (doResult) {
			std::string headers[] = {"Iteration","Time","Best Value"};
			results->put_headers(3, headers);
			std::string best_headers[] = {"Type","Iteration","Time","Value"};
			results->put_best_headers(4, best_headers);
		}
		
		SensorNetwork sensor_network(this->setup->instanceFilename);
		Solution *optimalIndividual = sensor_network.newSolution();
		optimalIndividual->generateBestTrivial();
		optimalIndividual->writeTextProgram(std::cout);
		Solution *bestIndividual = sensor_network.newSolution();
		Solution *currentIndividual = sensor_network.newSolution();
		Solution *newIndividual = sensor_network.newSolution();
		Solution *auxIndividual = sensor_network.newSolution();
		
		currentIndividual->initialize();
		currentIndividual->calculateFitness();
		(*bestIndividual) = (*currentIndividual);
		double bestFitness = bestIndividual->getFitness();
		
		if (doResult) {
			double values[] = {0.0,bestFitness};
			results->put_line("0",values);
			double best_values[] = {0.0,0.0,bestFitness};
			results->put_best_values("Best", best_values);
			std::cout << "\nExecuting TS... 0%";
		} else {
			std::cout <<
					std::setw(16) << "Iteration:" <<
					std::setw(16) << "Time(s):" <<
					std::setw(16) << "Fitness:" << std::endl <<
					std::setw(16) << "0" <<
					std::setw(16) << "0" <<
					std::setprecision(12) << std::setw(16) << bestFitness << std::endl;
		}
		
		int actualIteration = 0;
		int movesDone = 0, idx, bestLocalMove;
		double bestLocalFitness;
		vector<int> movements;
		movements = this->setup->movements;
		vector<int> tabuList;
		tabuList.reserve(movements.size());
		int tabuListSize = this->setup->tabuListMinSize;
		time_t initialTime = std::time(0);
		while ((std::time(0)-initialTime < this->setup->maximumExecutionTime) && (actualIteration < this->setup->maximumIterations)) {
			actualIteration++;
			movesDone = 0;
			bestLocalFitness = 1e100;
			bestLocalMove = -1;
			(*newIndividual) = (*currentIndividual);
			while (movesDone < this->setup->movesPerIteration) {
				(*auxIndividual) = (*currentIndividual);
				idx = (TS::intrand(TS::rng)%movements.size());
				operators::mutate(auxIndividual, movements[idx]);
				auxIndividual->calculateFitness();
				if (bestLocalFitness > auxIndividual->getFitness()) {
					bestLocalFitness = auxIndividual->getFitness();
					bestLocalMove = idx;
					(*newIndividual) = (*auxIndividual);
				}
				movesDone++;
			}
			(*currentIndividual) = (*newIndividual);
			if (currentIndividual->getFitness() < bestFitness) {
				bestFitness = currentIndividual->getFitness();
				(*bestIndividual) = (*currentIndividual);
				if (doResult) {
					double best_values[] = {actualIteration,(std::time(0)-initialTime),bestFitness};
					results->put_best_values("Best", best_values);
				} else {
					std::cout <<
						std::setw(16) << actualIteration <<
						std::setw(16) << (std::time(0)-initialTime) <<
						std::setprecision(12) << std::setw(16) << bestFitness << std::endl;
				}
			}
			
			tabuList.push_back(movements[bestLocalMove]);
			movements.erase(movements.begin()+bestLocalMove);
			
			if ((TS::intrand(TS::rng) % 2) == 0) {
				tabuListSize = this->setup->tabuListMinSize + (TS::intrand(TS::rng) %
					(this->setup->tabuListMaxSize - this->setup->tabuListMinSize) + 1);
			}
			while (tabuList.size() > tabuListSize) {
				movements.push_back(tabuList[0]);
				tabuList.erase(tabuList.begin());
			}
			
			if (doResult)
				std::cout << "\rExecuting TS... "
					<< ((int)(100*actualIteration/setup->maximumIterations)) << "%";
			if (actualIteration % 100 == 0) {
				if (doResult) {
					double values[] = {std::time(0)-initialTime, bestFitness};
					results->put_line(boost::lexical_cast<std::string>(actualIteration), values);
				} else {
					std::cout.precision(15);
					std::cout << actualIteration << "\t" << bestFitness << "\t" << std::time(0)-initialTime << std::endl;
					std::cout.flush();
				}
			}
		}
		
		if (!doResult) {
			std::cout << std::endl;
			std::cout << "Best Individual found: " << bestIndividual->getFitness() << std::endl;
			bestIndividual->writeTextProgram(std::cout);
			std::cout << std::endl;
			std::cout.flush();
		} else {
			results->getOutput() << std::endl;
			results->getOutput() << "Best Individual found: " << bestIndividual->getFitness() << std::endl;
			bestIndividual->writeTextProgram(results->getOutput());
			results->getOutput() << std::endl;
			results->getOutput().flush();
		}
		
		// Freeing memory.
		delete bestIndividual;
		delete currentIndividual;
		delete newIndividual;
		delete auxIndividual;
	}
	
}


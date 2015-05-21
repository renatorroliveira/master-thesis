/**
 * @file GeneticAlgorithm.cpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since GeneticNet 1.0
 * @date 10/07/2012
 */

#include <iomanip>
#include <boost/random.hpp>
#include <boost/lexical_cast.hpp>

#include "GeneticAlgorithm.hpp"
#include "GeneticOperators.hpp"

namespace geneticnet {
	namespace GA {
		boost::random::mt19937 rng;
		boost::random::uniform_int_distribution<> intrand;
		boost::random::uniform_real_distribution<> realrand;
		const char* setup_xml_name = "GeneticAlgorithmSetup";
	}
	
	GeneticAlgorithm::GeneticAlgorithm() {
		this->setup = new GA::Setup();
	}
	
	GeneticAlgorithm::GeneticAlgorithm(GA::Setup* gaSetup) : setup(gaSetup) {
		if (this->setup == 0) {
			std::cerr << "Error loading Genetic Algorithm: NULL setup.\n";
			abort();
		}
	}
	
	GeneticAlgorithm::GeneticAlgorithm(boost::archive::xml_iarchive& input) {
		GA::Setup* setup = new GA::Setup();
		input >> boost::serialization::make_nvp<GA::Setup>(GA::setup_xml_name, (*setup));
		this->setup = setup;
	}
	
	GeneticAlgorithm::~GeneticAlgorithm() {
		delete this->setup;
	}
	
	void GeneticAlgorithm::save_setup_to_xml(boost::archive::xml_oarchive& output) {
		output << boost::serialization::make_nvp<GA::Setup>(GA::setup_xml_name, *(this->setup));
	}
	void GeneticAlgorithm::load_setup_from_xml(boost::archive::xml_iarchive& input) {
		input >> boost::serialization::make_nvp<GA::Setup>(GA::setup_xml_name, *(this->setup));
	}
	
	void GeneticAlgorithm::execute(ResultManager * results) {
		bool doResult = (results != NULL);
		loggerpp::log("Starting Genetic Algorithm execution...", loggerpp::level::INFO, this);
		
		if (doResult) {
			std::string headers[] = {"Generation","Time","Best Value"};
			results->put_headers(3, headers);
			std::string best_headers[] = {"Type","Generation","Time","Value"};
			results->put_best_headers(4, best_headers);
		}
		
		SensorNetwork sensor_network(this->setup->instanceFilename);
		Solution *optimalIndividual = sensor_network.newSolution();
		optimalIndividual->generateBestTrivial();
		optimalIndividual->writeTextProgram(std::cout);
		Solution *bestIndividual = sensor_network.newSolution();
		Solution *newIndividual = sensor_network.newSolution();
		Solution **population = new Solution*[this->setup->populationSize];
		Solution **mattingPool = new Solution*[this->setup->populationSize];
		double bestFitness = 1e100;
		for (int ind = 0; ind < this->setup->populationSize; ind++) {
			population[ind] = sensor_network.newSolution();
			population[ind]->initialize();
			population[ind]->calculateFitness();
			if (population[ind]->getFitness() < bestFitness) {
				bestFitness = population[ind]->getFitness();
				*bestIndividual = *(population[ind]);
			}
			mattingPool[ind] = sensor_network.newSolution();
		}
		Solution * auxIndividual;
		if (doResult) {
			double values[] = {0.0,bestFitness};
			results->put_line("0",values);
			double best_values[] = {0.0,0.0,bestFitness};
			results->put_best_values("Best", best_values);
			std::cout << "\nExecuting GA... 0%";
		} else {
			bestIndividual->generateBestTrivial();
			bestIndividual->calculateFitness();
			std::cout << bestIndividual->getFitness() << std::endl;
			bestIndividual->writeTextProgram(std::cout);
			
			std::cout.precision(15);
			std::cout << "Initial population:" << std::endl;
			for (int i = 0; i < this->setup->populationSize; i++) {
				std::cout << "Individual " << i << ":\t" << population[i]->getFitness() << std::endl;
			}
			std::cout.flush();
		}
		int actualGeneration = 0;
		time_t initialTime = std::time(0);
		while ((std::time(0)-initialTime < this->setup->maximumExecutionTime) &&
			   (actualGeneration < this->setup->maximumGenerations)) {
			actualGeneration++;
			if (this->setup->elitism) {
				Solution *eliteIndividual = population[0];
				for (int ind = 1; ind < this->setup->populationSize; ind++) {
					if (eliteIndividual->getFitness() > population[ind]->getFitness())
						eliteIndividual = population[ind];
				}
				*(mattingPool[0]) = *eliteIndividual;
				if (eliteIndividual->getFitness() < bestFitness) {
					bestFitness = eliteIndividual->getFitness();
					*bestIndividual = *eliteIndividual;
				}
				
				// Evolve loop
				for (int ind = 1; ind < this->setup->populationSize; ind++) {
					if (GA::realrand(GA::rng) <= this->setup->crossoverRate) {
						operators::recombine(this->tournment(population), this->tournment(population), newIndividual);
					} else {
						(*newIndividual) = *(this->tournment(population));
					}
					if (GA::realrand(GA::rng) <= this->setup->mutationRate) {
						operators::mutate(newIndividual);
					}
					newIndividual->calculateFitness();
					if (newIndividual->getFitness() < bestFitness) {
						bestFitness = newIndividual->getFitness();
						*bestIndividual = *newIndividual;
						if (doResult) {
							double values[] = {actualGeneration, time(0)-initialTime, bestFitness};
							results->put_best_values("Best", values);
						}
					}
					auxIndividual = mattingPool[ind];
					mattingPool[ind] = newIndividual;
					newIndividual = auxIndividual;
				}
			} else {
				// Evolve loop
				for (int ind = 0; ind < this->setup->populationSize; ind++) {
					if (GA::realrand(GA::rng) <= this->setup->crossoverRate) {
						operators::recombine(this->tournment(population), this->tournment(population), newIndividual);
					} else {
						(*newIndividual) = *(this->tournment(population));
					}
					if (GA::realrand(GA::rng) <= this->setup->mutationRate) {
						operators::mutate(newIndividual);
					}
					newIndividual->calculateFitness();
					if (newIndividual->getFitness() < bestFitness) {
						bestFitness = newIndividual->getFitness();
						*bestIndividual = *newIndividual;
						if (doResult) {
							double values[] = {actualGeneration, time(0)-initialTime, bestFitness};
							results->put_best_values("Best", values);
						}
					}
					auxIndividual = mattingPool[ind];
					mattingPool[ind] = newIndividual;
					newIndividual = auxIndividual;
				}
			}
			Solution **auxPtr = mattingPool;
			mattingPool = population;
			population = auxPtr;
			
			if (doResult)
				std::cout << "\rExecuting GA... "
					<< ((int)(100*actualGeneration/setup->maximumGenerations)) << "%";
			if (actualGeneration % 100 == 0) {
				if (doResult) {
					double values[] = {time(0)-initialTime, bestFitness};
					results->put_line(boost::lexical_cast<std::string>(actualGeneration), values);
				} else {
					std::cout.precision(15);
					std::cout << actualGeneration << "\t" << bestFitness << "\t" << std::time(0)-initialTime << std::endl;
					for (int i = 0; i < this->setup->populationSize; i++) {
						std::cout << "Individual " << i << ":\t" << population[i]->getFitness() << std::endl;
					}
					std::cout.flush();
				}
			}
		}
		std::cout << "\n";
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
		
		// Frreing memory.
		delete newIndividual;
		for (int ind = 0; ind < this->setup->populationSize; ind++) {
			delete population[ind];
			delete mattingPool[ind];
		}
		delete [] population;
		delete [] mattingPool;
	}
	
	Solution* GeneticAlgorithm::tournment(Solution** population) {
		Solution* winner = population[GA::intrand(GA::rng) % this->setup->populationSize];
		for (int t = 0; t < this->setup->tournmentSize-1; t++) {
			int idx = GA::intrand(GA::rng) % this->setup->populationSize;
			if (population[idx]->getFitness() < winner->getFitness())
				winner = population[idx];
		}
		return winner;
	}
	
}


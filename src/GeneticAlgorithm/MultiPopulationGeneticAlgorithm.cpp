/**
 * @file MultiPopulationGeneticAlgorithm.cpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since GeneticNet 1.0
 * @date 04/06/2013
 */

#include <iomanip>
#include <boost/random.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/concept_check.hpp>
#include <sys/time.h>

#include "MultiPopulationGeneticAlgorithm.hpp"
#include "GeneticOperators.hpp"

namespace geneticnet {
	namespace MPGA {
		boost::random::mt19937 rng;
		boost::random::uniform_int_distribution<> intrand;
		boost::random::uniform_real_distribution<> realrand;
		const char* setup_xml_name = "MultiPopulationGeneticAlgorithmSetup";
	}
	
	MultiPopulationGeneticAlgorithm::MultiPopulationGeneticAlgorithm() {
		this->setup = new MPGA::Setup();
	}
	
	MultiPopulationGeneticAlgorithm::MultiPopulationGeneticAlgorithm(MPGA::Setup* mpgaSetup) : setup(mpgaSetup) {
		if (this->setup == 0) {
			std::cerr << "Error loading Multi Population Genetic Algorithm: NULL setup.\n";
			abort();
		}
	}
	
	MultiPopulationGeneticAlgorithm::MultiPopulationGeneticAlgorithm(boost::archive::xml_iarchive& input) {
		MPGA::Setup* setup = new MPGA::Setup();
		input >> boost::serialization::make_nvp<MPGA::Setup>(MPGA::setup_xml_name, (*setup));
		this->setup = setup;
	}
	
	MultiPopulationGeneticAlgorithm::~MultiPopulationGeneticAlgorithm() {
		delete this->setup;
	}
	
	void MultiPopulationGeneticAlgorithm::save_setup_to_xml(boost::archive::xml_oarchive& output) {
		output << boost::serialization::make_nvp<MPGA::Setup>(MPGA::setup_xml_name, *(this->setup));
	}
	void MultiPopulationGeneticAlgorithm::load_setup_from_xml(boost::archive::xml_iarchive& input) {
		input >> boost::serialization::make_nvp<MPGA::Setup>(MPGA::setup_xml_name, *(this->setup));
	}
	
	void MultiPopulationGeneticAlgorithm::execute(ResultManager * results) {
		bool doResult = (results != NULL);
		loggerpp::log("Starting Multi Population Genetic Algorithm execution...", loggerpp::level::INFO, this);
		
		if (doResult) {
			std::string headers[] = {"Generation","Time","Best Value"};
			results->put_headers(3, headers);
			std::string best_headers[] = {"Type","Generation","Time","Value"};
			results->put_best_headers(4, best_headers);
		}
		
		SensorNetwork sensor_network(this->setup->instanceFilename);
		Solution *optimalIndividual = sensor_network.newSolution();
		optimalIndividual->generateBestTrivial();
		optimalIndividual->calculateFitness();
		optimalIndividual->writeTextProgram(std::cout);
		
		std::cout << "nPops: " << this->setup->numberOfPopulations << std::endl
			<< "popSize: " << this->setup->populationSize[0] << std::endl
			<< "popStruct: " << this->setup->populationStructure[0] << std::endl
			<< "convergenceRate: " << this->setup->convergenceRate[0] << std::endl
			<< "crossOver: " << this->setup->crossoverRate[0] << std::endl
			<< "mutation: " << this->setup->mutationRate[0] << std::endl;
		std::cout.flush();
		
		Solution *bestIndividual = sensor_network.newSolution();
		Solution *newIndividual = sensor_network.newSolution();
		Solution *auxIndividual = sensor_network.newSolution();
		Solution ***populations = new Solution**[this->setup->numberOfPopulations];
		double bestFitness = 1e100;
		for (int pop = 0; pop < this->setup->numberOfPopulations; pop++) {
			populations[pop] = new Solution*[this->setup->populationSize[pop]];
			for (int ind = 0; ind < this->setup->populationSize[pop]; ind++) {
				populations[pop][ind] = sensor_network.newSolution();
				populations[pop][ind]->initialize();
				populations[pop][ind]->calculateFitness();
				if (populations[pop][ind]->getFitness() < bestFitness) {
					bestFitness = populations[pop][ind]->getFitness();
					*bestIndividual = *(populations[pop][ind]);
				}
			}
		}
		if (doResult) {
			double values[] = {0.0,bestFitness};
			results->put_line("0",values);
			double best_values[] = {0.0,0.0,bestFitness};
			results->put_best_values("Best", best_values);
			std::cout << "\nExecuting GA... 0%";
		} else {
			std::cout << std::setw(16) << "Individual:" <<
				std::setw(16) << "Generation:" <<
				std::setw(16) << "Convergence:" <<
				std::setw(16) << "Time(s):" <<
				std::setw(16) << "Fitness:" << std::endl;
		}
		
		int actualConvergence = 0;
		int actualGeneration = 0;
		int nIndividuals = 0;
		int totalIndividuals = 0;
		for (int pop = 0; pop < this->setup->numberOfPopulations; pop++) {
			totalIndividuals += this->setup->populationSize[pop];
		}
		
		int nIndividualsEval = 0;
		int crossAndMutImprovement = 0;
		int crossAndMutTotal = 0;
		int mutsThatStruggle = 0;
		int crossImprovement = 0;
		double mutationRate = 0.5f;
		bool mutated, crossedOver, whouldInsert;
		bool converged;
		timeval current, initial;
		gettimeofday(&initial, 0);
		gettimeofday(&current, 0);
		while ((current.tv_sec-initial.tv_sec < this->setup->maximumExecutionTime) &&
			   //(actualConvergence < this->setup->maximumConvergences)&&
			   (actualGeneration < this->setup->maximumGenerations)) {
			for (int pop = 0; pop < this->setup->numberOfPopulations; pop++) {
				nIndividualsEval = 0;
				crossAndMutImprovement = 0;
				crossAndMutTotal = 0;
				mutsThatStruggle = 0;
				crossImprovement = 0;
				actualConvergence++;
				do {
					converged = true;
					gettimeofday(&current, 0);
					if ((current.tv_sec-initial.tv_sec >= this->setup->maximumExecutionTime) ||
						//(actualConvergence < this->setup->maximumConvergences)&&
						(actualGeneration >= this->setup->maximumGenerations)) {
							break;
					}
					int amount = (this->setup->populationSize[pop]*this->setup->convergenceRate[pop]);
					for (int ind = 0; ind < amount; ind++) {
						int parents[2];
						this->select(populations, pop, parents);
						if (MPGA::realrand(MPGA::rng) <= this->setup->crossoverRate[pop]) {
							operators::recombine(populations[pop][parents[0]], populations[pop][parents[1]], newIndividual);
							crossedOver = true;
						} else {
							(*newIndividual) = (*(populations[pop][parents[0]]));
							crossedOver = false;
						}
						nIndividuals++;
						nIndividualsEval++;
						newIndividual->calculateFitness();
						if ((MPGA::realrand(MPGA::rng) <= this->setup->mutationRate[pop]) || (!crossedOver)) {
							if (this->willInsert(populations[pop], parents, newIndividual)) {
								whouldInsert = true;
								(*auxIndividual) = (*newIndividual);
							} else {
								whouldInsert = false;
							}
							operators::mutate(newIndividual);
							newIndividual->calculateFitness();
							if ((newIndividual->getFitness() > auxIndividual->getFitness()) && whouldInsert) {
								(*newIndividual) = (*auxIndividual);
								mutated = false;
							} else {
								crossAndMutTotal++;
								mutated = true;
							}
						} else
							mutated = false;
						if (this->insert(populations[pop], parents, newIndividual)) {
							this->arrange(populations, pop);
							if (mutated)
								crossAndMutImprovement++;
							else
								crossImprovement++;
							converged = false;
							if (newIndividual->getFitness() < bestFitness) {
								bestFitness = newIndividual->getFitness();
								*bestIndividual = *newIndividual;
								gettimeofday(&current, 0);
								if (doResult) {
									double values[] = {actualConvergence, current.tv_sec-initial.tv_sec, bestFitness};
									results->put_best_values("Best", values);
								} else {
									std::cout << std::setw(16) << nIndividuals <<
										std::setw(16) << actualGeneration <<
										std::setw(16) << actualConvergence <<
										std::setw(16) << (current.tv_sec-initial.tv_sec) <<
										std::setprecision(12) << std::setw(16) << bestFitness << std::endl;
								}
							}
						} else if (mutated && whouldInsert) {
							mutsThatStruggle++;
						}
					}
					actualGeneration = nIndividuals/totalIndividuals;
					std::cout.flush();
					if ((crossAndMutTotal > 0) && (crossAndMutImprovement > 0)
						&& (crossImprovement > 0) && (nIndividualsEval > 0)) {
						mutationRate = (0.5f*mutationRate) + (0.5f*(((double)crossAndMutImprovement/(double)(crossAndMutTotal))
							/ (((double)(crossImprovement+mutsThatStruggle)/(double)(nIndividualsEval-crossAndMutTotal))
							+ ((double)crossAndMutImprovement/(double)(crossAndMutTotal)))));
					}
				} while (!converged);
			}
			this->reinitialize(populations);
			this->migrate(populations);
			/*std::cout << "--- Total Individuals: " << nIndividualsEval << std::endl
				<< "Total Cross and Muts Applied: " << crossAndMutTotal << std::endl
				<< "Total Cross and Muts Improvement: " << crossAndMutImprovement << std::endl
				<< "Total Cross Improvement: " << crossImprovement << std::endl
				<< "Muts That Struggle: " << mutsThatStruggle << std::endl
				<< "Muts Struggle Rate (%): " << (100.0f*(double)mutsThatStruggle/((double)crossAndMutTotal)) << std::endl
				<< "Mutation Rate: " << mutationRate << std::endl
				<< "Cross Effectiveness (%): " << (100.0f*(double)crossImprovement/((double)(nIndividualsEval-crossAndMutTotal))) << std::endl
				<< "Mut Effectiveness (%): " << (100.0f*(double)crossAndMutImprovement/((double)(crossAndMutTotal))) << std::endl;
			std::cout.flush();*/
			
			if (doResult)
				std::cout << "\rExecuting GA... "
					<< ((int)(100*actualGeneration/setup->maximumGenerations)) << "%";
			if (actualGeneration % 100 == 0) {
				if (doResult) {
					gettimeofday(&current, 0);
					double values[] = {current.tv_sec-initial.tv_sec, bestFitness};
					results->put_line(boost::lexical_cast<std::string>(actualGeneration), values);
				}
			}
			gettimeofday(&current, 0);
		}
		std::cout << std::setw(16) << nIndividuals <<
			std::setw(16) << actualGeneration <<
			std::setw(16) << actualConvergence <<
			std::setw(16) << (current.tv_sec-initial.tv_sec) <<
			std::setprecision(12) << std::setw(16) << bestFitness << std::endl;
		std::cout << std::endl;
		if (!doResult) {
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
		delete bestIndividual;
		delete optimalIndividual;
		for (int pop = 0; pop < this->setup->numberOfPopulations; pop++) {
			for (int ind = 0; ind < this->setup->populationSize[pop]; ind++) {
				delete populations[pop][ind];
			}
			delete [] populations[pop];
		}
		delete [] populations;
	}
	
	/// Selects two parents, a cluster head and one of its child nodes.
	void MultiPopulationGeneticAlgorithm::select(Solution*** populations, int pop, int selectedParents[2]) {
		int numberOfClusters = (this->setup->populationSize[pop]-1)/this->setup->populationStructure[pop];
		selectedParents[0] = MPGA::intrand(MPGA::rng) % numberOfClusters;
		selectedParents[1] = selectedParents[0] + 1
			+ (MPGA::intrand(MPGA::rng) % this->setup->populationStructure[pop])
			+ (selectedParents[0] * (this->setup->populationStructure[pop] - 1));
	}
	
	bool MultiPopulationGeneticAlgorithm::insert(Solution** population, int selectedParents[2], Solution* child) {
		Solution* aux;
		if (population[selectedParents[1]]->getFitness() > child->getFitness()) {
			if (population[selectedParents[0]]->getFitness() > population[selectedParents[1]]->getFitness()) {
				(*(population[selectedParents[0]])) = (*child);
				/*for (int j = selectedParents[0]-1; j >= 0; j--) {
					if (population[j]->getFitness() > population[j+1]->getFitness()) {
						aux = population[j];
						population[j] = population[j+1];
						population[j+1] = aux;
					} else
						break;
				}*/
			} else {
				(*(population[selectedParents[1]])) = (*child);
			}
			return true;
		} else if (population[selectedParents[0]]->getFitness() > child->getFitness()) {
			if (population[selectedParents[0]]->getFitness() > population[selectedParents[1]]->getFitness()) {
				(*(population[selectedParents[0]])) = (*child);
			} else {
				(*(population[selectedParents[1]])) = (*child);
			}
			return true;
		}
		return false;
	}
	
	bool MultiPopulationGeneticAlgorithm::willInsert(Solution** population, int selectedParents[2], Solution* child) {
		if (population[selectedParents[0]]->getFitness() > child->getFitness()) {
			return true;
		} else if (population[selectedParents[1]]->getFitness() > child->getFitness()) {
			return true;
		}
		return false;
	}
	
	void MultiPopulationGeneticAlgorithm::arrange(Solution*** populations, int pop) {
		Solution* aux;
		for (int i = 1; i < this->setup->populationSize[pop]; i++) {
			for (int j = i-1; j >= 0; j--) {
				if (populations[pop][j]->getFitness() > populations[pop][j+1]->getFitness()) {
					aux = populations[pop][j];
					populations[pop][j] = populations[pop][j+1];
					populations[pop][j+1] = aux;
				}else
					break;
			}
		}
	}
	
	void MultiPopulationGeneticAlgorithm::arrange(Solution*** populations) {
		for (int pop = 0; pop < this->setup->numberOfPopulations; pop++) {
			this->arrange(populations, pop);
		}
	}
	
	void MultiPopulationGeneticAlgorithm::migrate(Solution*** populations) {
		Solution *aux;
		for (int pop = 0; pop < this->setup->numberOfPopulations; pop++) {
			if (pop == (this->setup->numberOfPopulations - 1)) {
				(*(populations[0][this->setup->populationSize[pop]-1])) = (*(populations[pop][0]));
			} else {
				(*(populations[pop+1][this->setup->populationSize[pop]-1])) = (*(populations[pop][0]));
			}
			this->arrange(populations, pop);
		}
	}
	
	void MultiPopulationGeneticAlgorithm::reinitialize(Solution*** populations) {
		for (int pop = 0; pop < this->setup->numberOfPopulations; pop++) {
			if (this->setup->restartType[pop] == 1) {
				for (int ind = 1; ind < this->setup->populationSize[pop]; ind++) {
					populations[pop][ind]->initialize();
					populations[pop][ind]->calculateFitness();
				}
				this->arrange(populations, pop);
			} else if (this->setup->restartType[pop] == 2) {
				for (int ind = 1; ind < this->setup->populationSize[pop]; ind++) {
					operators::mutate(populations[pop][ind]);
					populations[pop][ind]->repair();
					operators::mutate(populations[pop][ind]);
					populations[pop][ind]->repair();
					operators::mutate(populations[pop][ind]);
					populations[pop][ind]->calculateFitness();
				}
				this->arrange(populations, pop);
			} else if (this->setup->restartType[pop] == 3) {
				for (int ind = 1; ind < this->setup->populationSize[pop]; ind++) {
					operators::mutate(populations[pop][ind]);
					populations[pop][ind]->repair();
					operators::mutate(populations[pop][ind]);
					populations[pop][ind]->calculateFitness();
				}
				this->arrange(populations, pop);
			} else if (this->setup->restartType[pop] == 4) {
				for (int ind = 1; ind < this->setup->populationSize[pop]; ind++) {
					operators::mutate(populations[pop][ind]);
					populations[pop][ind]->calculateFitness();
				}
				this->arrange(populations, pop);
			}
		}
	}
	
	
}


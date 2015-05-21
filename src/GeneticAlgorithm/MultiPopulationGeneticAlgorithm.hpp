/**
 * @file MultiPopulationGeneticAlgorithm.hpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since GeneticNet 1.0
 * @date 04/06/2013
 */

#ifndef GRUBI_GENETICNET_MULTIPOPULATIONGENETICALGORITHM_HPP_INCLUDED
#define GRUBI_GENETICNET_MULTIPOPULATIONGENETICALGORITHM_HPP_INCLUDED

#include <iostream>
#include <cstdlib>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>
#include "AbstractMetaheuristic.hpp"
#include "ResultManager.hpp"
#include "Solution.hpp"
#include "SensorNetwork.hpp"
#include <GMPI/Logger.hpp>

namespace geneticnet {
	namespace MPGA {
		class Setup;
	}
	/**
	 * @brief Implements the Multi Population Genetic Algorithm.
	 */
	class MultiPopulationGeneticAlgorithm : public AbstractMetaheuristic {
	public:
		MultiPopulationGeneticAlgorithm();
		MultiPopulationGeneticAlgorithm(MPGA::Setup* mpgaSetup);
		MultiPopulationGeneticAlgorithm(boost::archive::xml_iarchive& input);
		~MultiPopulationGeneticAlgorithm();
	
		virtual void execute(ResultManager * results = NULL);
		virtual void save_setup_to_xml(boost::archive::xml_oarchive& output);
		virtual void load_setup_from_xml(boost::archive::xml_iarchive& input);	
		
		void select(Solution*** populations, int pop, int selectedParents[2]);
		bool insert(Solution** population, int selectedParents[2], Solution* child);
		bool willInsert(Solution** population, int selectedParents[2], Solution* child);
		void arrange(Solution*** populations, int pop);
		void arrange(Solution*** populations);
		void migrate(Solution*** populations);
		void reinitialize(Solution*** populations);
	protected:
		MPGA::Setup *setup;
	};

	/// The Multi Population Genetic Algorithm specific structures.
	namespace MPGA {
		/**
		 * @brief This is a bean class for the Multi Population Genetic Algorithm's execution parameters.
		 */
		class Setup {
			friend class geneticnet::AbstractMetaheuristic;
			friend class geneticnet::MultiPopulationGeneticAlgorithm;
			friend class boost::serialization::access;
		public:
			/// Constructs the setup and define default values for alll parameters.
			Setup() {
				this->numberOfPopulations = 3;
				this->populationSize.reserve(3);
				this->populationStructure.reserve(3);
				this->convergenceRate.reserve(3);
				this->crossoverRate.reserve(3);
				this->crossoverType.reserve(3);
				this->mutationRate.reserve(3);
				this->mutationType.reserve(3);
				this->restartType.reserve(3);
				for (int p = 0; p < 3; p++) {
					this->populationSize.push_back(7);
					this->populationStructure.push_back(2);
					this->convergenceRate.push_back(2.0);
					this->crossoverRate.push_back(0.6);
					this->crossoverType.push_back(0);
					this->mutationRate.push_back(0.7);
					this->mutationType.push_back(0);
					this->restartType.push_back(0);
				}
				
				this->maximumConvergences = 100;
				this->maximumGenerations = 2000;
				this->maximumExecutionTime = 3600.0;
				this->migrationType = 0;
				this->instanceFilename = "";
			}
			/// Destruct parameters, if necessary.
			~Setup() {}
		protected:
			/// Number of populations.
			int numberOfPopulations;
			/// Number of individuals of each population.
			vector<int> populationSize;
			/// Structure of each populations.
			vector<int> populationStructure;
			/// Convergence rate of each populations.
			vector<double> convergenceRate;
			/// Crossover rate of each populations.
			vector<double> crossoverRate;
			/// Crossover type of each populations.
			vector<int> crossoverType;
			/// Mutation rate of each populations.
			vector<double> mutationRate;
			/// Mutation Type of each populations.
			vector<int> mutationType;
			/// The type of population restart for each population.
			vector<int> restartType;
			/// Maximum number of genetic convergences for the MPGA.
			double maximumConvergences;
			/// Maximum number of genetic generations for the MPGA.
			double maximumGenerations;
			/// Maximum execution time (real time) for the MPGA.
			double maximumExecutionTime;
			/// The type of population migration to use.
			int migrationType;
			/// Instance (Sensor Network) configuration file path.
			std::string instanceFilename;
			
			/// Genetic Algorithm's setup serialization.
			template<class Archive>
			void serialize(Archive & archive, const unsigned int version) {
				archive & BOOST_SERIALIZATION_NVP(numberOfPopulations);
				archive & BOOST_SERIALIZATION_NVP(populationSize);
				archive & BOOST_SERIALIZATION_NVP(populationStructure);
				archive & BOOST_SERIALIZATION_NVP(convergenceRate);
				archive & BOOST_SERIALIZATION_NVP(crossoverRate);
				archive & BOOST_SERIALIZATION_NVP(crossoverType);
				archive & BOOST_SERIALIZATION_NVP(mutationRate);
				archive & BOOST_SERIALIZATION_NVP(mutationType);
				archive & BOOST_SERIALIZATION_NVP(restartType);
				archive & BOOST_SERIALIZATION_NVP(maximumConvergences);
				archive & BOOST_SERIALIZATION_NVP(maximumGenerations);
				archive & BOOST_SERIALIZATION_NVP(maximumExecutionTime);
				archive & BOOST_SERIALIZATION_NVP(migrationType);
				archive & BOOST_SERIALIZATION_NVP(instanceFilename);
			}
		};
	}
}
BOOST_CLASS_VERSION(geneticnet::MPGA::Setup, 1)

#endif

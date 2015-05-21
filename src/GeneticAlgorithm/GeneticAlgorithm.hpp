/**
 * @file GeneticAlgorithm.hpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since GeneticNet 1.0
 * @date 10/07/2012
 */

#ifndef GRUBI_GENETICNET_GENETICALGORITHM_HPP_INCLUDED
#define GRUBI_GENETICNET_GENETICALGORITHM_HPP_INCLUDED

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
	namespace GA {
		class Setup;
	}
	/**
	 * @brief Implements the canonical Genetic Algorithm.
	 */
	class GeneticAlgorithm : public AbstractMetaheuristic {
	public:
		GeneticAlgorithm();
		GeneticAlgorithm(GA::Setup* gaSetup);
		GeneticAlgorithm(boost::archive::xml_iarchive& input);
		~GeneticAlgorithm();
	
		virtual void execute(ResultManager * results = NULL);
		virtual void save_setup_to_xml(boost::archive::xml_oarchive& output);
		virtual void load_setup_from_xml(boost::archive::xml_iarchive& input);
		
		Solution* tournment(Solution** population);
	protected:
		GA::Setup *setup;
	};

	/// The Genetic Algorithm specific structures.
	namespace GA {
		/**
		 * @brief This is a bean class for the Genetic Algorithm's execution parameters.
		 */
		class Setup {
			friend class geneticnet::AbstractMetaheuristic;
			friend class geneticnet::GeneticAlgorithm;
			friend class boost::serialization::access;
		public:
			/// Constructs the setup and define default values for all parameters.
			Setup() {
				this->populationSize = 8;
				this->maximumGenerations = 1000000;
				this->maximumExecutionTime = 3600.0;
				this->crossoverRate = 0.7;
				this->mutationRate = 0.05;
				this->elitism = true;
				this->tournmentSize = 3;
				this->instanceFilename = "";
			}
			/// Destruct parameters, if necessary.
			~Setup() {}
		protected:
			/// Number of individuals in the population.
			int populationSize;
			/// Maximum number of population's generations.
			int maximumGenerations;
			/// Maximum execution time (real time) for the Genetic Algorithm.
			double maximumExecutionTime;
			/// The rate to apply crossover to generate the new individual.
			double crossoverRate;
			/// The rate to apply mutation to the new generated individual.
			double mutationRate;
			/// Use elitism criterias?
			bool elitism;
			/// Number of individuals selected to tournment.
			int tournmentSize;
			/// Instance (Sensor Network) configuration file path.
			std::string instanceFilename;
			
			/// Genetic Algorithm's setup serialization.
			template<class Archive>
			void serialize(Archive & archive, const unsigned int version) {
				archive & BOOST_SERIALIZATION_NVP(populationSize);
				archive & BOOST_SERIALIZATION_NVP(maximumGenerations);
				archive & BOOST_SERIALIZATION_NVP(maximumExecutionTime);
				archive & BOOST_SERIALIZATION_NVP(crossoverRate);
				archive & BOOST_SERIALIZATION_NVP(mutationRate);
				archive & BOOST_SERIALIZATION_NVP(elitism);
				archive & BOOST_SERIALIZATION_NVP(tournmentSize);
				archive & BOOST_SERIALIZATION_NVP(instanceFilename);
			}
		};
	}
}
BOOST_CLASS_VERSION(geneticnet::GA::Setup, 1)

#endif

/**
 * @file TabuSearch.hpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since GeneticNet 1.0
 * @date 06/06/2013
 */

#ifndef GRUBI_GENETICNET_TABUSEARCH_HPP_INCLUDED
#define GRUBI_GENETICNET_TABUSEARCH_HPP_INCLUDED

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
	namespace TS {
		class Setup;
	}
	/**
	 * @brief Implements the local search algorithm Tabu Search.
	 */
	class TabuSearch : public AbstractMetaheuristic {
	public:
		TabuSearch();
		TabuSearch(TS::Setup* tsSetup);
		TabuSearch(boost::archive::xml_iarchive& input);
		~TabuSearch();
	
		virtual void execute(ResultManager * results = NULL);
		virtual void save_setup_to_xml(boost::archive::xml_oarchive& output);
		virtual void load_setup_from_xml(boost::archive::xml_iarchive& input);
		
	protected:
		TS::Setup *setup;
	};

	/// The Tabu Search specific structures.
	namespace TS {
		/**
		 * @brief This is a bean class for the Tabu Search's execution parameters.
		 */
		class Setup {
			friend class geneticnet::AbstractMetaheuristic;
			friend class geneticnet::TabuSearch;
			friend class boost::serialization::access;
		public:
			/// Constructs the setup and define default values for alll parameters.
			Setup() {
				this->movesPerIteration = 40;
				this->numberOfMovements = 4;
				this->movements.reserve(4);
				for (int m = 0; m < 4; m++) {
					this->movements.push_back(m+1);
				}
				this->tabuListMinSize = 1;
				this->tabuListMaxSize = 3;
				this->maximumIterations = 2000;
				this->maximumExecutionTime = 3600.0;
				this->instanceFilename = "";
			}
			/// Destruct parameters, if necessary.
			~Setup() {}
		protected:
			/// Moves per iteration before selecting best move.
			int movesPerIteration;
			/// Number of local movements.
			int numberOfMovements;
			/// List of local movements to be used.
			vector<int> movements;
			/// Tabu list minimum size.
			int tabuListMinSize;
			/// Tabu list maximum size.
			int tabuListMaxSize;
			/// Maximum number of population's generations.
			int maximumIterations;
			/// Maximum execution time (real time) for the Genetic Algorithm.
			double maximumExecutionTime;
			/// Instance (Sensor Network) configuration file path.
			std::string instanceFilename;
			
			/// Tabu Search's setup serialization.
			template<class Archive>
			void serialize(Archive & archive, const unsigned int version) {
				archive & BOOST_SERIALIZATION_NVP(movesPerIteration);
				archive & BOOST_SERIALIZATION_NVP(numberOfMovements);
				archive & BOOST_SERIALIZATION_NVP(movements);
				archive & BOOST_SERIALIZATION_NVP(tabuListMinSize);
				archive & BOOST_SERIALIZATION_NVP(tabuListMaxSize);
				archive & BOOST_SERIALIZATION_NVP(maximumIterations);
				archive & BOOST_SERIALIZATION_NVP(maximumExecutionTime);
				archive & BOOST_SERIALIZATION_NVP(instanceFilename);
			}
		};
	}
}
BOOST_CLASS_VERSION(geneticnet::TS::Setup, 1)

#endif

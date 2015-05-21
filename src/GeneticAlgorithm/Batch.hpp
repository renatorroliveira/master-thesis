/**
 * @file Batch.hpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since GeneticNet 1.0
 * @date 10/07/2012
 */

#ifndef GRUBI_GENETICNET_BATCH_HPP_INCLUDED
#define GRUBI_GENETICNET_BATCH_HPP_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_archive_exception.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/nvp.hpp>
#include "AbstractMetaheuristic.hpp"
#include <GMPI/Logger.hpp>

namespace geneticnet {
	class Batch {
		friend class boost::serialization::access;
	public:
		Batch(std::string batchFilename);
		~Batch();
		
		/// Executes this loaded batch.
		void execute();
	protected:
		/// Number of configurations loaded in this batch.
		int numberOfConfigurations;
		/// Number of configurations loaded in this batch.
		std::string *resultsFilenames;
		/// Loaded IDs for metaheuristics loaded.
		int *metaheuristicID;
		/// Number of executions of each metaheuristic loaded.
		int *numberOfExecutions;
		/// Loaded metaheuristics, instantiated from factory method at AbstractMetaheuristic.
		AbstractMetaheuristic **metaheuristics;
	};
}

#endif

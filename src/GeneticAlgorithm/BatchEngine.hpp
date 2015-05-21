/**
 * @file BatchEngine.hpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since GeneticNet 1.0
 * @date 10/07/2012
 */

#ifndef GRUBI_GENETICNET_BATCHENGINE_HPP_INCLUDED
#define GRUBI_GENETICNET_BATCHENGINE_HPP_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/nvp.hpp>
#include "Batch.hpp"
#include <GMPI/Logger.hpp>

namespace geneticnet {
	class BatchEngine {
	public:
		BatchEngine(std::string mainBatchFilename);
		~BatchEngine();
		
		void execute();
		void save_to_xml(std::string mainBatchFilename);
	protected:
		int numberOfBatchs;
		std::string *batchsFilenames;
		std::string *batchsNames;
		Batch **batchs;
	};
}

#endif

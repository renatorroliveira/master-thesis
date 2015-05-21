/**
 * @file BatchEngine.cpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since GeneticNet 1.0
 * @date 10/07/2012
 */

#include "BatchEngine.hpp"

namespace geneticnet 
{
	BatchEngine::BatchEngine(std::string mainBatchFilename) 
	{
		loggerpp::log("Loading the main batch file: " + mainBatchFilename, loggerpp::level::INFO); /* Load the main batch file */
		std::ifstream inputstream(mainBatchFilename.c_str(), std::ios::in);
		
		if (!inputstream)
			loggerpp::log("Cannot open the main batch file.", loggerpp::level::FATAL_ERROR);
		
		try {
			boost::archive::xml_iarchive inputarchive(inputstream);
			inputarchive & BOOST_SERIALIZATION_NVP(numberOfBatchs);			/* Read the number of batchs */
			
			if (numberOfBatchs <= 0)
				loggerpp::log("Zero batchs to load.", loggerpp::level::FATAL_ERROR);

			batchsFilenames = new std::string[numberOfBatchs];
			batchsNames = new std::string[numberOfBatchs];
			for (int b = 0; b < this->numberOfBatchs; b++) 
			{
				/* Read <batchsFilenames> */
				inputarchive & boost::serialization::make_nvp<std::string>("batchsFilenames", batchsFilenames[b]);
				/* Read <batchsNames> */
				inputarchive & boost::serialization::make_nvp<std::string>("batchsNames", batchsNames[b]);
			}
		} catch (std::exception ex) {
			loggerpp::log(ex.what(), loggerpp::level::FATAL_ERROR);
		}

		loggerpp::log("Main batch file loaded, loading batchs...", loggerpp::level::INFO);
		
		// Creating the batchs.
		this->batchs = new Batch*[numberOfBatchs];
		for (int b = 0; b < this->numberOfBatchs; b++) {
			this->batchs[b] = new Batch(this->batchsFilenames[b]);
		}
	}
	

	BatchEngine::~BatchEngine() {
		delete [] batchsFilenames;
		delete [] batchsNames;
		for (int b = 0; b < this->numberOfBatchs; b++) {
			delete batchs[b];
		}
		delete [] batchs;
	}
	
	void BatchEngine::execute() {
		loggerpp::log("Starting the main batch execution...", loggerpp::level::INFO);
		for (int b = 0; b < numberOfBatchs; b++) {
			batchs[b]->execute();
		}
	}
	
	void BatchEngine::save_to_xml(std::string mainBatchFilename) {
		std::ofstream outputstream(mainBatchFilename.c_str(), std::ios::out);
		if (!outputstream) {
			loggerpp::log("Cannot write the main batch file.", loggerpp::level::SEVERE);
			return;
		}
		try {
			boost::archive::xml_oarchive outputarchive(outputstream);
			outputarchive & BOOST_SERIALIZATION_NVP(numberOfBatchs);
			for (int b = 0; b < this->numberOfBatchs; b++) {
				outputarchive & boost::serialization::make_nvp<std::string>("batchsFilenames", batchsFilenames[b]);
				outputarchive & boost::serialization::make_nvp<std::string>("batchsNames", batchsNames[b]);
			}
		} catch (std::exception ex) {
			loggerpp::log(ex.what(), loggerpp::level::SEVERE);
		}
	}
}

/**
 * @file Batch.cpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since GeneticNet 1.0
 * @date 10/07/2012
 */

#include "Batch.hpp"

using namespace loggerpp;

namespace geneticnet {
	Batch::Batch(std::string batchFilename) 
	{
		log("Loading batch: " + batchFilename + " ...", level::INFO); 	/* Load the batch. */
		std::ifstream inputstream(batchFilename.c_str(), std::ios::in);
		
		if (!inputstream)
			log("Cannot open a batch file: " + batchFilename, level::FATAL_ERROR);
		
		try {
			boost::archive::xml_iarchive inputarchive(inputstream);
			inputarchive & BOOST_SERIALIZATION_NVP(numberOfConfigurations);	/* Read the number of configurations */

			if (numberOfConfigurations <= 0)
				log("Zero configurations to load for a batch: " + batchFilename, level::FATAL_ERROR);

			metaheuristicID = new int[numberOfConfigurations];						/* One metaheuristic id for every configuration. */
			resultsFilenames = new std::string[numberOfConfigurations]; 			/* One result file name for every configuration. */
			numberOfExecutions = new int[numberOfConfigurations];					/* One execution for every configuration. */
			metaheuristics = new AbstractMetaheuristic*[numberOfConfigurations]; 	/* One metaheuristic for every configuration */

			for (int conf = 0; conf < numberOfConfigurations; conf++) 	/* For every configuration */
			{
				inputarchive & boost::serialization::make_nvp<int>("metaheuristicID", metaheuristicID[conf]);		
				inputarchive & boost::serialization::make_nvp<int>("numberOfExecutions", numberOfExecutions[conf]);
				inputarchive & boost::serialization::make_nvp<std::string>("resultFilename", resultsFilenames[conf]);
				metaheuristics[conf] = AbstractMetaheuristic::for_metaheuristic_id(metaheuristicID[conf]);	/* Get the metaheuristic */
				metaheuristics[conf]->load_setup_from_xml(inputarchive);	/* Load the metaheuristic setup */
			}
		} catch (boost::archive::archive_exception ex) {
			log("Caught exception during batch loading: "+ batchFilename, level::SEVERE);
			log(ex.what(), loggerpp::level::FATAL_ERROR);
		}
	}


	Batch::~Batch() {
		if (metaheuristicID != 0) {
			delete [] metaheuristicID;
			delete [] resultsFilenames;
			delete [] numberOfExecutions;
			for (int conf = 0; conf < numberOfConfigurations; conf++) {
				if (metaheuristics[conf] != 0)
					delete metaheuristics[conf];
			}
			delete [] metaheuristics;
		}
	}
	
	void Batch::execute() 
	{
		log("Starting batch execution for metaheuristics...", level::INFO);
		for (int conf = 0; conf < numberOfConfigurations; conf++) {
			ResultManager * results;
			if (this->resultsFilenames[conf] == "off")
				results = 0;
			else
				results = new ResultManager(this->resultsFilenames[conf], this->numberOfExecutions[conf]);
			for (int exec = 0; exec < numberOfExecutions[conf]; exec++) {
				if (results != 0)
					results->startExecution();
				metaheuristics[conf]->execute(results);
				if (results != 0)
					results->finishExecution();
			}
			results->calculate_averages();
			delete results;
		}
	}
}

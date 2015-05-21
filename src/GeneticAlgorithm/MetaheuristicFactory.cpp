/**
 * This is the metaheuristic factory. Every new method must register it instatialization
 * by an unique ID here.
 */

#include <GMPI/Logger.hpp>
#include "AbstractMetaheuristic.hpp"

// Include headers files of all metaheuristics.
#include "GeneticAlgorithm.hpp"
#include "MultiPopulationGeneticAlgorithm.hpp"
#include "TabuSearch.hpp"

namespace geneticnet {
	using namespace loggerpp;
	AbstractMetaheuristic* AbstractMetaheuristic::for_metaheuristic_id(const int id) {
		switch (id) {
			case 1:
				return new GeneticAlgorithm();
			case 2:
				return new MultiPopulationGeneticAlgorithm();
			case 3:
				return new TabuSearch();
			default:
				log("Invalid metaheuristic ID at MetaheuristicFactory.", level::FATAL_ERROR);
		}
	}
}

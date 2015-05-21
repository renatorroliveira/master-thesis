/**
 * @file SensorNetwork.cpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since GeneticNet 1.0
 * @date 11/07/2012
 */

#include "SensorNetwork.hpp"

using namespace loggerpp;

namespace geneticnet {
	SensorNetwork::SensorNetwork(std::string configFilename) {
		log("Loading sensor network from configuration: "+configFilename, level::INFO, this);
		tinynode::Simulator* simulator = new tinynode::Simulator(configFilename);
		this->simulator = simulator;
		basicSolution = new Solution(simulator);
	}
	
	SensorNetwork::~SensorNetwork() {
		delete this->simulator;
		delete this->basicSolution;
	}
	
	Solution* SensorNetwork::newSolution() const {
		Solution* sol = new Solution(this->simulator);
		return sol;
	}
}

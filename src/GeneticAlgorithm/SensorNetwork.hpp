/**
 * @file SensorNetwork.hpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since GeneticNet 1.0
 * @date 11/07/2012
 */

#ifndef GRUBI_GENETICNET_SENSORNETWORK_HPP_INCLUDED
#define GRUBI_GENETICNET_SENSORNETWORK_HPP_INCLUDED

// STL Stuff
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>

// Boost stuff.
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

// Custom libs stuff.
#include <GMPI/Logger.hpp>
#include <TinyNode/Simulator.hpp>

// Internal stuff.
#include "Solution.hpp"

namespace geneticnet {
	class SensorNetwork {
	protected:
		tinynode::Simulator *simulator;
		Solution* basicSolution;
	public:
		SensorNetwork(std::string configFilename);
		~SensorNetwork();
		
		Solution* newSolution() const;
	};
}

#endif

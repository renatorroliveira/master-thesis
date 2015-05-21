/**
 * @file Simulator.hpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since TinyNode 1.0
 * @date 17/07/2012
 */

#ifndef GRUBI_TINYNODE_SIMULATOR_HPP_INCLUDED
#define GRUBI_TINYNODE_SIMULATOR_HPP_INCLUDED

#include <string>
#include <vector>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/version.hpp>
#include <Odin/Trigger.hpp>

#include "Network.hpp"
#include "Statistics.hpp"
#include "EventTrigger.hpp"

namespace tinynode {
	namespace simulation {
		class Setup;
	}
	class Simulator {
	protected:
		Network network;
		simulation::Setup *setup;
		double elapsedTime;
		int numberOfEvents;
		std::vector<SensorNode*> *nodes;
		std::vector<SensorNode*> sinkNodes;
		std::vector<SensorNode*> nodesToTrigger;
		std::vector<EventTrigger*>* eventsToTrigger;
	public:
		Simulator(std::string confiFilename);
		~Simulator();
		
		void simulate(Statistics* statistic, std::vector<odin::Trigger*>& program);
		const simulation::Setup* getSetup() const { return this->setup; }
		const double getElapsedTIme() const { return this->elapsedTime; }
		const int getNumberOfEvents() const { return this->numberOfEvents; }
	};
	
	namespace simulation {
		class Setup {
			friend class boost::serialization::access;
		public:
			double simulationMaxTime;
			double nodesSleepCycle;
			int numberOfNodes;
			int nodesActiveMemorySize;
			int nodesPassiveMemorySize;
			
			std::vector<int> sinkNodes;
			std::vector<EventTrigger*> eventTriggers;
			
			Setup() { }
			~Setup() { }
			
		protected:
			template<class Archive>
			void serialize(Archive& archive, const unsigned int version) {
				archive & BOOST_SERIALIZATION_NVP(simulationMaxTime);
				archive & BOOST_SERIALIZATION_NVP(nodesSleepCycle);
				archive & BOOST_SERIALIZATION_NVP(numberOfNodes);
				archive & BOOST_SERIALIZATION_NVP(nodesActiveMemorySize);
				archive & BOOST_SERIALIZATION_NVP(nodesPassiveMemorySize);
				archive & BOOST_SERIALIZATION_NVP(sinkNodes);
				archive & BOOST_SERIALIZATION_NVP(eventTriggers);
			}
		};
	}
}
BOOST_CLASS_VERSION(tinynode::simulation::Setup, 0)

#endif

/**
 * @file Statistics.hpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since TinyNode 1.0
 * @date 17/07/2012
 */

#ifndef GRUBI_TINYNODE_STATISTICS_HPP_INCLUDED
#define GRUBI_TINYNODE_STATISTICS_HPP_INCLUDED

#include <vector>
#include "SensorNode.hpp"
#include "EventTrigger.hpp"

namespace tinynode {
	namespace simulation {
		class Setup;
	}
	class Statistics {
	public:
		virtual void packetSent(double currentTime, SensorNode* from, SensorNode* to, int event, bool value) = 0;
		virtual void simulationCycle(double currentTime) = 0;
		virtual void simulationStart(simulation::Setup* setup,
									 std::vector<SensorNode*>* nodes,
									 std::vector<SensorNode*>* sinkNodes,
									 EventTrigger** eventsToTrigger,
									 int numberOfEvents) = 0;
		virtual void simulationEnd(double currentTime) = 0;
		virtual ~Statistics() { }
	};
}

#endif

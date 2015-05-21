/**
 * Implements a statistics class for the TinyNode.
 * @file SensorNetworkStatistics.hpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since GeneticNet 1.0
 * @date 18/07/2012
 */

#ifndef GRUBI_GENETICNET_SENSORNETWORKSTATISTICS_HPP_INCLUDED
#define GRUBI_GENETICNET_SENSORNETWORKSTATISTICS_HPP_INCLUDED

#include <vector>
#include <TinyNode/SensorNode.hpp>
#include <TinyNode/Simulator.hpp>
#include <TinyNode/Statistics.hpp>

namespace geneticnet {
	class Solution;
	class SensorNetworkStatistics : public tinynode::Statistics {
		friend class geneticnet::Solution;
	protected:
		int numberOfSimulations;
		int total_events_active_before_any_trigger;
		int total_packets_sent_away_from_sink;
		int total_packets_sent_before_any_trigger;
		int total_packets_sent;
		int total_triggers_not_reached_any_sink;
		double total_triggers_min_dist_to_sink;
		
		int events_active_before_any_trigger;
		int packets_sent_away_from_sink;
		int packets_sent_before_any_trigger;
		int packets_sent;
		int triggers_not_reached_any_sink;
		double triggers_min_dist_to_sink;
		
		tinynode::simulation::Setup* simulationSetup;
		std::vector<tinynode::SensorNode*>* nodes;
		std::vector<tinynode::SensorNode*>* sinkNodes;
		tinynode::EventTrigger** eventsToTrigger;
		int numberOfEvents;
	public:
		SensorNetworkStatistics();
		~SensorNetworkStatistics();
		virtual void packetSent(double currentTime, tinynode::SensorNode* from, tinynode::SensorNode* to, int event, bool value);
		virtual void simulationCycle(double currentTime);
		virtual void simulationStart(tinynode::simulation::Setup* setup,
									 std::vector<tinynode::SensorNode*>* nodes,
									 std::vector<tinynode::SensorNode*>* sinkNodes,
									 tinynode::EventTrigger** eventsToTrigger,
									 int numberOfEvents);
		virtual void simulationEnd(double currentTime);
		void postProcessStatistics();
	};
}

#include "Solution.hpp"

#endif

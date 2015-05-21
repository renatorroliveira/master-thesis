/**
 * @brief Implements a statistics class for the TinyNode.
 * @file SensorNetworkStatistics.cpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since GeneticNet 1.0
 * @date 18/07/2012
 */

#include <GMPI/Logger.hpp>
#include "SensorNetworkStatistics.hpp"

namespace geneticnet {
	SensorNetworkStatistics::SensorNetworkStatistics() {
		this->numberOfSimulations = 0;
		
		this->total_events_active_before_any_trigger = 0;
		this->total_packets_sent_away_from_sink = 0;
		this->total_packets_sent_before_any_trigger = 0;
		this->total_packets_sent = 0;
		this->total_triggers_not_reached_any_sink = 0;
		this->total_triggers_min_dist_to_sink = 0.0f;
		
		this->events_active_before_any_trigger = 0;
		this->packets_sent_away_from_sink = 0;
		this->packets_sent_before_any_trigger = 0;
		this->packets_sent = 0;
		this->triggers_not_reached_any_sink = 0;
		this->triggers_min_dist_to_sink = 0.0f;
		
		this->simulationSetup = 0;
		this->nodes = 0;
		this->sinkNodes = 0;
		this->eventsToTrigger = 0;
		this->numberOfEvents = 0;
	}
	
	SensorNetworkStatistics::~SensorNetworkStatistics() {
		
	}
	
	void SensorNetworkStatistics::packetSent(double currentTime, tinynode::SensorNode* from, tinynode::SensorNode* to, int event, bool value) {
		this->packets_sent++;
		
		for (int evt = 0; value && (evt < this->numberOfEvents); evt++) {
			if (event == this->eventsToTrigger[evt]->getEventID()) {
				if (currentTime < this->eventsToTrigger[evt]->getOcurrenceTime())
					this->packets_sent_before_any_trigger++;
				break;
			}
		}
		
		double minFromDistance = 1e100;
		double minToDistance = 1e100;
		std::vector<tinynode::SensorNode*>::iterator it;
		double aux;
		for (it = this->sinkNodes->begin(); it != this->sinkNodes->end(); it++) {
			aux = from->getPosition() % (*it)->getPosition();
			if (aux < minFromDistance) {
				minFromDistance = aux;
			}
			aux = to->getPosition() % (*it)->getPosition();
			if (aux < minToDistance) {
				minToDistance = aux;
			}
		}
		if (minFromDistance - minToDistance < -0.1f)
			this->packets_sent_away_from_sink++;
	}
	
	void SensorNetworkStatistics::simulationCycle(double currentTime) {
		for (int e = 0; e < this->numberOfEvents; e++) {
			if (currentTime < this->eventsToTrigger[e]->getOcurrenceTime()) {
				int eventId = this->eventsToTrigger[e]->getEventID();
				for (int n = 0; n < this->nodes->size(); n++) {
					if (this->nodes->at(n)->getPassiveMemory(eventId))
						this->events_active_before_any_trigger++;
				}
			}
		}
	}
	
	void SensorNetworkStatistics::simulationStart(tinynode::simulation::Setup* setup,
									 std::vector<tinynode::SensorNode*>* nodes,
									 std::vector<tinynode::SensorNode*>* sinkNodes,
									 tinynode::EventTrigger** eventsToTrigger,
									 int numberOfEvents) {
		this->numberOfSimulations++;
		
		this->total_events_active_before_any_trigger += this->events_active_before_any_trigger;
		this->total_packets_sent_away_from_sink += this->packets_sent_away_from_sink;
		this->total_packets_sent_before_any_trigger += this->packets_sent_before_any_trigger;
		this->total_packets_sent += this->packets_sent;
		this->total_triggers_not_reached_any_sink += this->triggers_not_reached_any_sink;
		this->total_triggers_min_dist_to_sink += this->triggers_min_dist_to_sink;
		
		this->events_active_before_any_trigger = 0;
		this->packets_sent_away_from_sink = 0;
		this->packets_sent_before_any_trigger = 0;
		this->packets_sent = 0;
		this->triggers_not_reached_any_sink = 0;
		this->triggers_min_dist_to_sink = 0.0f;
		
		this->simulationSetup = setup;
		this->nodes = nodes;
		this->sinkNodes = sinkNodes;
		this->eventsToTrigger = eventsToTrigger;
		this->numberOfEvents = numberOfEvents;
	}
	
	void SensorNetworkStatistics::simulationEnd(double currentTime) {
		for (int e = 0; e < this->numberOfEvents; e++) {
			bool reached = false;
			int eventId = this->eventsToTrigger[e]->getEventID();
			std::vector<tinynode::SensorNode*>::iterator sink;
			for (sink = this->sinkNodes->begin(); sink != this->sinkNodes->end(); sink++) {
				if ((*sink)->getPassiveMemory(eventId))
					reached = true;
			}
			if (!reached) {
				this->triggers_not_reached_any_sink++;
				double minDist = 1e100;
				for (int n = 0; n < this->nodes->size(); n++) {
					if (this->nodes->at(n)->getPassiveMemory(eventId)) {
						for (sink = this->sinkNodes->begin(); sink != this->sinkNodes->end(); sink++) {
							double dist = this->nodes->at(n)->getPosition() % (*sink)->getPosition();
							if (dist < minDist)
								minDist = dist;
						}
					}
				}
				this->triggers_min_dist_to_sink += minDist;
			}
		}
	}
	
	void SensorNetworkStatistics::postProcessStatistics() {
		this->total_events_active_before_any_trigger += this->events_active_before_any_trigger;
		this->total_packets_sent_away_from_sink += this->packets_sent_away_from_sink;
		this->total_packets_sent_before_any_trigger += this->packets_sent_before_any_trigger;
		this->total_packets_sent += this->packets_sent;
		this->total_triggers_not_reached_any_sink += this->triggers_not_reached_any_sink;
		this->total_triggers_min_dist_to_sink += this->triggers_min_dist_to_sink;
		
		this->events_active_before_any_trigger = this->total_events_active_before_any_trigger;
		this->packets_sent_away_from_sink = this->total_packets_sent_away_from_sink;
		this->packets_sent_before_any_trigger = this->total_packets_sent_before_any_trigger;
		this->packets_sent = this->total_packets_sent;
		this->triggers_not_reached_any_sink = this->total_triggers_not_reached_any_sink;
		this->triggers_min_dist_to_sink = this->total_triggers_min_dist_to_sink / this->numberOfSimulations;
	}
}

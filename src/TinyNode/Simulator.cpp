/**
 * @file Simulator.cpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since TinyNode 1.0
 * @date 17/07/2012
 */

#include <algorithm>
#include <fstream>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_archive_exception.hpp>
#include <boost/serialization/nvp.hpp>
#include <GMPI/Logger.hpp>
#include <Odin/Command.hpp>
#include <Odin/Executor.hpp>
#include "OdinConnector.hpp"
#include "Simulator.hpp"

namespace tinynode{
	Simulator::Simulator(std::string confiFilename) {
		std::ifstream in(confiFilename.c_str(), std::ios::in);
		if (!in) {
			loggerpp::log("Cannot open configuration file: "+confiFilename, loggerpp::level::FATAL_ERROR, this);
		}
		try {
			boost::archive::xml_iarchive input(in);
			input >> boost::serialization::make_nvp("SimulationSetup", this->setup);
			input >> boost::serialization::make_nvp("Network", this->network);
		} catch (boost::archive::xml_archive_exception ex) {
			loggerpp::log("Exception while loading configurations file: "+confiFilename+" ("+ex.what()+")", loggerpp::level::FATAL_ERROR, this);
		}
		this->nodes = network.getAllNodes();
		// Sorting of the event triggers.
		std::sort(setup->eventTriggers.begin(), setup->eventTriggers.end(), EventTrigger::less_than);
		int last = setup->eventTriggers.at(0)->getEventID();
		this->numberOfEvents = 1;
		for (int t = 0; t < setup->eventTriggers.size(); t++) {
			if (last != setup->eventTriggers.at(t)->getEventID()) {
				last = setup->eventTriggers.at(t)->getEventID();
				this->numberOfEvents++;
			}
		}
		this->eventsToTrigger = new std::vector<EventTrigger*>[this->numberOfEvents];
		last = setup->eventTriggers.at(0)->getEventID();
		int evt = 0;
		for (int t = 0; t < setup->eventTriggers.size(); t++) {
			if (last != setup->eventTriggers.at(t)->getEventID()) {
				last = setup->eventTriggers.at(t)->getEventID();
				evt++;
			}
			this->eventsToTrigger[evt].push_back(setup->eventTriggers.at(t));
		}
		
		// Getting the sink nodes.
		std::vector<int>::iterator it;
		for (it = this->setup->sinkNodes.begin(); it != this->setup->sinkNodes.end(); it++) {
			SensorNode* sink = 0;
			for (int n = 0; n < this->nodes->size(); n++) {
				if (this->nodes->at(n)->getID() == (*it))
					sink = this->nodes->at(n);
			}
			if (sink == 0)
				loggerpp::log("No sink node found at received nodes vector.\n", loggerpp::level::FATAL_ERROR, this);
			this->sinkNodes.push_back(sink);
		}
		
		// Storing the nodes where the events will occur.
		std::vector<EventTrigger*>::iterator it2;
		for (it2 = setup->eventTriggers.begin(); it2 != setup->eventTriggers.end(); it2++) {
			EventTrigger* trigger = *it2;
			SensorNode* node = 0;
			for (int n = 0; n < this->nodes->size(); n++) {
				if (this->nodes->at(n)->getID() == trigger->getNodeID())
					node = this->nodes->at(n);
			}
			if (node == 0)
				loggerpp::log("No node found to event that will occur.", loggerpp::level::FATAL_ERROR, this);
			this->nodesToTrigger.push_back(node);
		}
	}
	
	Simulator::~Simulator() {
		delete this->nodes;
		delete [] this->eventsToTrigger;
	}
	
	void Simulator::simulate(Statistics* statistic, std::vector<odin::Trigger*>& program) {
		OdinConnector odinConnector(&(this->network), statistic, this);
		OdinExecutor codeExecutor(program, statistic, &odinConnector);
		for (int t = 0; t < this->eventsToTrigger[0].size(); t++) {
			// Restarting nodes state and deploying application.
			this->network.deployApplication(codeExecutor);
			// Setup of event triggers.
			EventTrigger **trigger = new EventTrigger*[this->numberOfEvents];
			for (int e = 0; e < this->numberOfEvents; e++) {
				trigger[e] = this->eventsToTrigger[e].at(t);
				for (int n = 0; n < nodes->size(); n++) {
					if ((*nodes)[n]->getID() == trigger[e]->getNodeID()) {
						trigger[e]->setMemory(nodes->at(n)->odinActiveMemory);
						break;
					}
				}
				if (trigger[e]->getEvent() == NULL)
					loggerpp::log("No node/event found for a given event trigger.\n", loggerpp::level::FATAL_ERROR, this);
			}
			
			// Configures sink information.
			this->network.configureSink(this->sinkNodes.at(0));
		
			statistic->simulationStart(this->setup, this->nodes, &(this->sinkNodes), trigger, this->numberOfEvents);
			this->elapsedTime = 0.0f;
			while (this->elapsedTime <= setup->simulationMaxTime) {
				// Triggering the triggers.
				for (int e = 0; e < this->numberOfEvents; e++) {
					trigger[e]->trigger(elapsedTime);
				}
			
				// Executing the code in all nodes.
				std::vector<SensorNode*>::iterator iter;
				for (iter = nodes->begin(); iter != nodes->end(); iter++) {
					SensorNode* node = *iter;
					if (!node->executeApplication(&odinConnector)) {
						loggerpp::log("Error executing application on sensor node.", loggerpp::level::FATAL_ERROR, this);
					}
				}
				statistic->simulationCycle(this->elapsedTime);
				this->elapsedTime += setup->nodesSleepCycle;
			}
			statistic->simulationEnd(this->elapsedTime);
		}
	}
}

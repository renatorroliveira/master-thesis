/**
 * @file OdinConnector.cpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since TinyNode 1.0
 * @date 17/07/2012
 */

#include <iostream>
#include <GMPI/Logger.hpp>
#include "OdinConnector.hpp"
#include "Network.hpp"
#include "SensorNode.hpp"
#include "Simulator.hpp"
#include "Statistics.hpp"

using std::cout;

namespace tinynode {

	OdinConnector::OdinConnector(Network *network, Statistics *statistic, Simulator* simulator) 
	{
		this->network = network;
		this->statistic = statistic;
		this->simulator = simulator;
		
		if (network == NULL || statistic == NULL || simulator == NULL)
			loggerpp::log("Network/statistic/simulator pointer(s) is(are) NULL.", loggerpp::level::FATAL_ERROR, this);
		
		std::vector<SensorNode*>* nodes = this->network->getAllNodes();
		std::vector<SensorNode*>::iterator it;
		std::vector<SensorNode*>::iterator it2;
		
		/* Construct the adjacency lists */		
		for (it = nodes->begin(); it != nodes->end(); it++) 
		{
			std::vector<SensorNode*>* adjacents = new std::vector<SensorNode*>();
			for (it2 = nodes->begin(); it2 != nodes->end(); it2++) 
			{
				if (it != it2) 
				{
					if (((*it)->getPosition() % (*it2)->getPosition()) <= (*it)->getTransmissionRange()) {
						adjacents->push_back(*it2);
					}
				}
			}
			
			if (adjacents->size() <= 0)
				loggerpp::log("Disconnected node found!\n", loggerpp::level::SEVERE, this);
			
			std::pair<int, std::vector<SensorNode*>*> tuple;
			tuple.first = (*it)->getID();
			tuple.second = adjacents;
			this->adjacencyLists.insert(tuple);	/*	Add the new adjacency list. */
		}
		delete nodes;
	}


	OdinConnector::~OdinConnector() {
		std::map<int, std::vector<SensorNode*>*>::iterator it;
		for (it = this->adjacencyLists.begin(); it != this->adjacencyLists.end(); it++) {
			delete (*it).second;
		}
	}

	
	void OdinConnector::sendDown (int ev, bool what) {
		SensorNode* node = this->actualContext;
		
		if (node == NULL) {
			loggerpp::log("The context is NULL.", loggerpp::level::SEVERE, this);
			return;
		}

		Position pos = node->getPosition();			/* actual context node position */
		
		SensorNode *closer = 0;
		std::vector<SensorNode*>* adjacents = this->adjacencyLists.at(node->getID());	/* Get tha adjacency list of the actual node */
		
		std::vector<SensorNode*>::iterator it;
		for (it = adjacents->begin(); it != adjacents->end(); it++) 
		{
			if (pos.getY() - (*it)->getPosition().getY() > 0.01f)	/* if the actual node is above the *it node */
			{
				if (closer == 0)
					closer = (*it);
				//else if (closer->getPosition() % pos > (*it)->getPosition() % pos)
				else if (closer->getPosition() % pos > closer->getPosition() % (*it)->getPosition())
					closer = (*it);
			}
		}
		if (closer != NULL) {
			closer->setPassiveMemory(ev,what);	/* Change the passive event in the adjacent node */
			this->statistic->packetSent(this->simulator->getElapsedTIme(), this->actualContext, closer, ev, what);
		}
	}


	void OdinConnector::sendUp (int ev, bool what) 
	{
		SensorNode* node = this->actualContext;
		if (node == NULL) {
			loggerpp::log("The context is NULL.", loggerpp::level::SEVERE, this);
			return;
		}

		Position pos = node->getPosition();		/* actual context node position */

		SensorNode *closer = 0;
		std::vector<SensorNode*>* adjacents = this->adjacencyLists.at(node->getID()); /* Get tha adjacency list of the actual node */

		std::vector<SensorNode*>::iterator it;
		for (it = adjacents->begin(); it != adjacents->end(); it++) 
		{
			if (pos.getY() - (*it)->getPosition().getY() < -0.01f) 	/* if the actual node is below the *it node */
			{
				if (closer == 0)
					closer = (*it);
				//else if (closer->getPosition() % pos > (*it)->getPosition() % pos)
				else if (closer->getPosition() % pos > closer->getPosition() % (*it)->getPosition())
					closer = (*it);
			}
		}

		if (closer != NULL) {
			closer->setPassiveMemory(ev,what); /* Change the passive event in the adjacent node */
			this->statistic->packetSent(this->simulator->getElapsedTIme(), this->actualContext, closer, ev, what);
		}
	}


	void OdinConnector::sendRight (int ev, bool what) 
	{
		SensorNode* node = this->actualContext;

		if (node == NULL) {
			loggerpp::log("The context is NULL.", loggerpp::level::SEVERE, this);
			return;
		}

		Position pos = node->getPosition();	/* actual context node position */

		SensorNode *closer = 0;
		std::vector<SensorNode*>* adjacents = this->adjacencyLists.at(node->getID());	/* Get tha adjacency list of the actual node */

		std::vector<SensorNode*>::iterator it;
		for (it = adjacents->begin(); it != adjacents->end(); it++) 
		{
			if (pos.getX() - (*it)->getPosition().getX() < -0.01f)	/* If the actual node is on the left of the *it node */
			{
				if (closer == 0)
					closer = (*it);
				//else if (closer->getPosition() % pos > (*it)->getPosition() % pos) /*  */
				else if (closer->getPosition() % pos > closer->getPosition() % (*it)->getPosition())
					closer = (*it);
			}
		}

		if (closer != NULL) 
		{
			closer->setPassiveMemory(ev,what); /* Change the passive event in the adjacent node */
			this->statistic->packetSent(this->simulator->getElapsedTIme(), this->actualContext, closer, ev, what);
		}
	}


	void OdinConnector::sendLeft (int ev, bool what) 
	{
		SensorNode* node = this->actualContext;

		if (node == NULL) {
			loggerpp::log("The context is NULL.", loggerpp::level::SEVERE, this);
			return;
		}

		Position pos = node->getPosition();

		SensorNode *closer = 0;
		std::vector<SensorNode*>* adjacents = this->adjacencyLists.at(node->getID());

		std::vector<SensorNode*>::iterator it;
		for (it = adjacents->begin(); it != adjacents->end(); it++) 
		{
			if (pos.getX() - (*it)->getPosition().getX() > 0.01f) 	/* If the actual node (pos) is on the right of the *it node */
			{
				if (closer == 0)
					closer = (*it);
				//else if (closer->getPosition() % pos > (*it)->getPosition() % pos)
				else if (closer->getPosition() % pos > closer->getPosition() % (*it)->getPosition())
					closer = (*it);
			}
		}

		if (closer != NULL) 
		{
			closer->setPassiveMemory(ev,what); /* Change the passive event in the adjacent node */
			this->statistic->packetSent(this->simulator->getElapsedTIme(), this->actualContext, closer, ev, what);
		}
	}
}

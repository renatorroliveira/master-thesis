/**
 * @file OdinConnector.hpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since TinyNode 1.0
 * @date 17/07/2012
 */

#ifndef GRUBI_TINYNODE_ODINCONNECTOR_HPP_INCLUDED
#define GRUBI_TINYNODE_ODINCONNECTOR_HPP_INCLUDED

#include <map>
#include <vector>

#include <Odin/Connector.hpp>

namespace tinynode {
	class Network;
	class SensorNode;
	class Simulator;
	class Statistics;
	class OdinConnector : public odin::Connector {
	protected:
		Network *network;
		Statistics *statistic;
		Simulator* simulator;
		SensorNode* actualContext;
		std::map<int, std::vector<SensorNode*>*> adjacencyLists; /* Map the node id to the node adjacency list. */
		
	public:
		OdinConnector(Network *network, Statistics *statistic, Simulator* simulator);
		~OdinConnector();
		
		virtual void sendDown (int ev, bool what);
		virtual void sendUp (int ev, bool what);
		virtual void sendRight (int ev, bool what);
		virtual void sendLeft (int ev, bool what);
	
		void setContext(SensorNode* context) { this->actualContext = context; }
	};
}

#endif

/**
 * @file OdinExecutor.hpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since TinyNode 1.0
 * @date 23/07/2012
 */

#ifndef GRUBI_TINYNODE_ODINEXECUTOR_HPP_INCLUDED
#define GRUBI_TINYNODE_ODINEXECUTOR_HPP_INCLUDED

#include <vector>

#include <Odin/Command.hpp>
#include <Odin/Event.hpp>
#include <Odin/Memory.hpp>
#include <Odin/Connector.hpp>
#include <Odin/Trigger.hpp>

#include <GMPI/Logger.hpp>

#include "OdinConnector.hpp"

namespace tinynode {
	class SensorNode;
	class Statistics;
	/**
		* Executor
		*/
	class OdinExecutor {
	public:
		OdinExecutor(std::vector<odin::Trigger*>& program, Statistics *statistic, OdinConnector* connector);
		~OdinExecutor();
		
		void execute(SensorNode* executor, odin::Memory& active, odin::Memory& passive);
	protected:
		std::vector<odin::Trigger*> commandList;
		Statistics *statistic;
		OdinConnector *connector;
	};
	
}
#endif
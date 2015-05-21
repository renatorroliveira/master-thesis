/**
 * @file OdinExecutor.cpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since TinyNode 1.0
 * @date 23/07/2012
 */

#include "OdinExecutor.hpp"

namespace tinynode {
	OdinExecutor::OdinExecutor(std::vector<odin::Trigger*>& program, Statistics *statistic, OdinConnector* connector) :
			commandList(program) {
		this->statistic = statistic;
		this->connector = connector;
	}
	OdinExecutor::~OdinExecutor(){
		
	}
	
	//Função membro completa?????????
	void OdinExecutor::execute(SensorNode* executor, odin::Memory& active, odin::Memory& passive){
		connector->setContext(executor);
		std::vector<odin::Trigger*>::iterator iter;
		for(iter = this->commandList.begin(); iter != this->commandList.end(); ++iter){
			odin::Trigger* trigger = (*iter);
			if (trigger->test(active, passive)){
				for (int cmd = 0; cmd < trigger->getCommands()->size(); cmd++) {
					trigger->getCommands()->at(cmd)->execute(active, passive, this->connector);
				}
			}
		}
		connector->setContext(NULL);
	}
}

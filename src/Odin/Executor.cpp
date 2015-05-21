/**
 * @file Executor.cpp
 * @author Renato R. R. de Oliveira (renatorro@comp.ufla.br)
 * @author Ariel Marques
 * @version 1.0
 * @since Odin 1.0
 * @date 09/07/2012
 */

#include "Executor.hpp"

namespace odin {
	Executor::Executor(std::vector<Trigger* >& list) :
	commandList(list) {
		this->firstTime = true;
	}
	Executor::~Executor(){

	}

	//Função membro completa?????????
	void Executor::execute(int source, Memory& active, Memory& passive, Connector* connector){
		std::vector<odin::Trigger*>::iterator iter;
		for(iter = this->commandList.begin(); iter != this->commandList.end(); ++iter){
			odin::Trigger* trigger = (*iter);
			if (trigger->test(active, passive)){
				for (int cmd = 0; cmd < trigger->getCommands()->size(); cmd++) {
					trigger->getCommands()->at(cmd)->execute(active, passive, connector);
				}
			}
		}
	}
}

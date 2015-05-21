/**
 * @file DownEventCommand.cpp
 * @author Renato R. R. de Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since Odin 1.0
 * @date 07/07/2012
 */

#include "DownEventCommand.hpp"

namespace odin {
	DownEventCommand::DownEventCommand(int event) {
		this->event = event;
	}

	DownEventCommand::DownEventCommand(const DownEventCommand& command) {
		this->event = command.event;
	}

	DownEventCommand::~DownEventCommand() {}

	void DownEventCommand::execute(Memory& active, Memory& passive, Connector * connector) {
		passive.setMemoryAt(this->event, false);
	}

	Command* DownEventCommand::clone() {
		return new DownEventCommand(*this);
	}

	std::string DownEventCommand::toString() const {
		std::stringstream out;
		out << "passive[" << this->event << "] = false";
		return out.str();
	}
}

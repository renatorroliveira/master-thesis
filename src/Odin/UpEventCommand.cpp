/**
 * @file UpEventCommand.cpp
 * @author Renato R. R. de Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since Odin 1.0
 * @date 07/07/2012
 */

#include "UpEventCommand.hpp"

namespace odin {
	UpEventCommand::UpEventCommand(int eventNumber) {
		this->eventNumber = eventNumber;
	}

	UpEventCommand::UpEventCommand(const UpEventCommand& command) {
		this->eventNumber = command.eventNumber;
	}

	UpEventCommand::~UpEventCommand() {}

	void UpEventCommand::execute(Memory& active, Memory& passive, Connector * connector) {
		passive.setMemoryAt(this->eventNumber, true);
	}

	Command* UpEventCommand::clone() {
		return new UpEventCommand(*this);
	}

	std::string UpEventCommand::toString() const {
		std::stringstream out;
		out << "passive[" << this->eventNumber << "] = true";
		return out.str();
	}
}

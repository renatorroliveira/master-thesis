/**
 * \file SendUpCommand.cpp
 * \author Renato R. R. de Oliveira (renatorro@comp.ufla.br)
 * \author Ariel Marques
 * \version 1.0
 * \since Odin 1.0
 * \date 08/07/2012
 */

#include "SendUpCommand.hpp"
namespace odin {
	SendUpCommand::SendUpCommand(const SendUpCommand& command) {
		this->eventNumber = command.eventNumber;
		this->destination = command.destination->clone();
	}
	SendUpCommand::SendUpCommand(int eventNumber, Destination* destination) {
		this->eventNumber = eventNumber;
		this->destination = destination;
	}
	SendUpCommand::~SendUpCommand(){}

	Command* SendUpCommand::clone() {
		return new SendUpCommand(*this);
	}
	void SendUpCommand::execute(Memory& active, Memory& passive, Connector * connector) {
		this->destination->execute(this->eventNumber, true, connector);
	}

	std::string SendUpCommand::toString() const {
		std::stringstream out;
		out << "send(" << this->eventNumber << ", true, " << this->destination->toString() << ")";
		return out.str();
	}
}

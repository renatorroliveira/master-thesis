/**
 * \file DestinationUp.cpp
 * \author Renato R. R. de Oliveira (renatorro@comp.ufla.br)
 * \author Ariel Marques
 * \version 1.0
 * \since Odin 1.0
 * \date 16/07/2012
 */

#include "DestinationUp.hpp"
namespace odin {
	void DestinationUp::execute(int event, bool what, Connector *connector) {
		connector->sendUp(event, what);
	}
	Destination* DestinationUp::clone() {
		return new DestinationUp(*this);
	}

	std::string DestinationUp::toString() const {
		return "'up'";
	}
}

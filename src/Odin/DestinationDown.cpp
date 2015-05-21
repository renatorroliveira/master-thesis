/**
 * \file DestinationDown.cpp
 * \author Renato R. R. de Oliveira (renatorro@comp.ufla.br)
 * \author Ariel Marques
 * \version 1.0
 * \since Odin 1.0
 * \date 16/07/2012
 */
#include "DestinationDown.hpp"
namespace odin {
	void DestinationDown::execute(int event, bool what, Connector *connector) {
		connector->sendDown(event, what);
	}
	Destination* DestinationDown::clone() {
		return new DestinationDown(*this);

	}

	std::string DestinationDown::toString() const {
		return "'down'";
	}

}

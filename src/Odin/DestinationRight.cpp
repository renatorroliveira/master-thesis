/**
 * \file DestinationRight.cpp
 * \author Renato R. R. de Oliveira (renatorro@comp.ufla.br)
 * \author Ariel Marques
 * \version 1.0
 * \since Odin 1.0
 * \date 16/07/2012
 */

#include "DestinationRight.hpp"

namespace odin {
	void DestinationRight::execute (int event, bool what, Connector *connector) {
		connector->sendRight(event,what);
	}


	Destination* DestinationRight::clone(){
		return new  DestinationRight(*this);
	}

	std::string DestinationRight::toString() const {
		return "'right'";
	}
}

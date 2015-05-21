/**
 * \file DestinationLef.cpp
 * \author Renato R. R. de Oliveira (renatorro@comp.ufla.br)
 * \author Ariel Marques
 * \version 1.0
 * \since Odin 1.0
 * \date 16/07/2012
 */

#include "DestinationLeft.hpp"
namespace odin {
	void DestinationLeft::execute (int event, bool what, Connector *connector){
		connector->sendLeft(event, what);
	}
	Destination* DestinationLeft::clone() {
		return new DestinationLeft(*this);
	}

	std::string DestinationLeft::toString() const {
		return "'left'";
	}
}

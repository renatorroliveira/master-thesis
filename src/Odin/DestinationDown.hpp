 /**
 * \file Destination.hpp
 * \author Renato R. R. de Oliveira (renatorro@comp.ufla.br)
 * \author Ariel Marques
 * \version 1.0
 * \since Odin 1.0
 * \date 16/07/2012
 */

#ifndef GRUBI_ODIN_DESTINATIONDOWN_HPP
#define GRUBI_ODIN_DESTINATIONDOWN_HPP

#include <string>
#include <sstream>
#include "Destination.hpp"
#include "Connector.hpp"
#include <iostream>

namespace odin {
	/**
	 * Destinatário para baixo.
	 */
	class DestinationDown : public Destination {
	public:
		DestinationDown() {}
		~DestinationDown() {}
		virtual Destination* clone();
		virtual void execute(int event, bool what, Connector *connector);
		virtual std::string toString() const;
	};
}

#endif

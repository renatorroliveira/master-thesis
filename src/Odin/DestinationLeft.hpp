/**
 * \file DestinationLeft.hpp
 * \author Renato R. R. de Oliveira (renatorro@comp.ufla.br)
 * \author Ariel Marques
 * \version 1.0
 * \since Odin 1.0
 * \date 16/07/2012
 */

#ifndef GRUBI_ODIN_DESTINATIONLEFT_HPP
#define GRUBI_ODIN_DESTINATIONLEFT_HPP

#include <string>
#include <sstream>
#include "Destination.hpp"
#include "Connector.hpp"

namespace odin {
	/**
	 * Envia o pacote para esquerda
	 */
	class DestinationLeft : public Destination{
	public:
		DestinationLeft() {}
		~DestinationLeft() {}
		virtual Destination* clone();
		virtual void execute (int event, bool what, Connector *connector);
		virtual std::string toString() const;
	};
}



#endif

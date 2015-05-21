/**
 * \file DestinationUp.hpp
 * \author Renato R. R. de Oliveira (renatorro@comp.ufla.br)
 * \author Ariel Marques
 * \version 1.0
 * \since Odin 1.0
 * \date 16/07/2012
 */

#ifndef GRUBI_ODIN_DESTINATIONUP_HPP
#define GRUBI_ODIN_DESTINATIONUP_HPP

#include <string>
#include <sstream>
#include "Destination.hpp"
#include "Connector.hpp"
namespace odin {
	/**
	 * envia pacota para posiç?o geograficamenta acima do nó corrente
	 */
	class DestinationUp : public Destination{
	public:
		DestinationUp() {}
		~DestinationUp() {}
		virtual Destination* clone();
		virtual void execute(int event, bool what, Connector *connector);
		virtual std::string toString() const;
	};


}

#endif

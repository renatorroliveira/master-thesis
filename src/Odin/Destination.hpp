/**
 * \file Destination.hpp
 * \author Renato R. R. de Oliveira (renatorro@comp.ufla.br)
 * \author Ariel Marques
 * \version 1.0
 * \since Odin 1.0
 * \date 16/07/2012
 */
#ifndef GRUBI_ODIN_DESTINATION_HPP
#define GRUBI_ODIN_DESTINATION_HPP

#include <string>
#include "Connector.hpp"
namespace odin {
	/**
	* Interface que define uma implementação de destino.
	*/
	class Destination {
	public:
		virtual ~Destination() { }
		virtual Destination* clone() = 0;
		virtual void execute(int event, bool what, Connector *connector) = 0;
		virtual std::string toString() const = 0;
	};
}

#endif

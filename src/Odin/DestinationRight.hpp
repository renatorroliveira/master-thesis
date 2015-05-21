/**
 * \file DestinationRight.hpp
 * \author Renato R. R. de Oliveira (renatorro@comp.ufla.br)
 * \author Ariel Marques
 * \version 1.0
 * \since Odin 1.0
 * \date 16/07/2012
 */

#ifndef GRUBI_ODIN_DESTINATIONRIGHT_HPP_INCLUDED
#define GRUBI_ODIN_DESTINATIONRIGHT_HPP_INCLUDED

#include <string>
#include <sstream>
#include "Destination.hpp"
#include "Connector.hpp"

namespace odin {
		/**
		 * Envia o pacote para a posição a geograficamente a direita
		 * do nó corrente
		 */
		class DestinationRight : public Destination {
		public:
			DestinationRight() {}
			~DestinationRight() {}
			virtual Destination* clone();
			virtual void execute(int event, bool what, Connector *connector);
			virtual std::string toString() const;
		};
}

#endif

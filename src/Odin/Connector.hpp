/**
 * \file Connector.hpp
 * \author Renato R. R. de Oliveira (renatorro@comp.ufla.br)
 * \author Ariel Marques
 * \version 1.0
 * \since Odin 1.0
 * \date 16/07/2012
 */

#ifndef GRUBI_ODIN_CONNECTOR_HPP
#define GRUBI_ODIN_CONNECTOR_HPP
namespace odin {
	/**
	 * Interface para conectar-se ao GRUBIX
	 */
	class Connector {
	public:
		Connector() {}
		virtual ~Connector() {}
		virtual void sendDown (int ev, bool what) = 0;
		virtual void sendUp (int ev, bool what) = 0;
		virtual void sendRight (int ev, bool what) = 0;
		virtual void sendLeft (int ev, bool what) = 0;
	};
}
#endif

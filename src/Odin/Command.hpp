/**
 * @file Command.hpp
 * @author Renato R. R. de Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since Odin 1.0
 * @date 06/07/2012
 */

#ifndef GRUBI_ODIN_COMMAND_HPP_INCLUDED
#define GRUBI_ODIN_COMMAND_HPP_INCLUDED


#include <string>
#include <vector>
#include "Connector.hpp"
#include "Event.hpp"
#include "Memory.hpp"


namespace odin {
	class Command {
	public:
		virtual ~Command() {}
		virtual void execute(Memory& active, Memory& passive, Connector * connector) = 0;
		virtual Command* clone() = 0;
		virtual int getEvent1() const = 0;
		virtual int getEvent2() const = 0;
		virtual std::string toString() const = 0;
	};
}

#endif

/**
 * @file UpEventCommand.hpp
 * @author Renato R. R. de Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since Odin 1.0
 * @date 07/07/2012
 */

#ifndef GRUBI_ODIN_UPEVENTCOMMAND_HPP_INCLUDED
#define GRUBI_ODIN_UPEVENTCOMMAND_HPP_INCLUDED

#include <string>
#include <sstream>
#include "Command.hpp"


namespace odin {
	/**
	 * @brief Sets up (turn on) an event.
	 */
	class UpEventCommand : public Command {
	public:
		UpEventCommand(int eventNumber);
		UpEventCommand(const UpEventCommand& command);
		~UpEventCommand();
		virtual void execute(Memory& active, Memory& passive, Connector * connector);
		virtual Command* clone();
		virtual int getEvent1() const { return this->eventNumber; }
		virtual int getEvent2() const { return 0; }
		virtual std::string toString() const;
	protected:
		int eventNumber;
	};
}

#endif

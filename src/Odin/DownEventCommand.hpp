/**
 * @file DownEventCommand.hpp
 * @author Renato R. R. de Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since Odin 1.0
 * @date 07/07/2012
 */

#ifndef GRUBI_ODIN_DOWNEVENTCOMMAND_HPP_INCLUDED
#define GRUBI_ODIN_DOWNEVENTCOMMAND_HPP_INCLUDED

#include <string>
#include <sstream>
#include "Command.hpp"


namespace odin {
	/**
	 * @brief Sets down (turn off) an event.
	 */
	class DownEventCommand : public Command {
	public:
		DownEventCommand(int event);
		DownEventCommand(const DownEventCommand& command);
		~DownEventCommand();
		virtual void execute(Memory& active, Memory& passive, Connector * connector);
		virtual Command* clone();
		virtual int getEvent1() const { return event; }
		virtual int getEvent2() const { return 0; }
		virtual std::string toString() const;
	protected:
		int event;
	};
}

#endif

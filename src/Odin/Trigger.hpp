/**
 * @file Trigger.hpp
 * @author Renato R. R. de Oliveira (renatorro@comp.ufla.br)
 * @version 2.0
 * @since Odin 1.0
 * @date 13/08/2012
 */

#ifndef GRUBI_ODIN_TRIGGER_HPP_INCLUDED
#define GRUBI_ODIN_TRIGGER_HPP_INCLUDED

#include <string>
#include <sstream>
#include <vector>
#include "Command.hpp"
#include "Memory.hpp"


namespace odin {
	/**
	 * @brief Class that respresents a trigger (bassically a composed event trigger).
	 */
	class Trigger {
	public:
		Trigger(int, int, int, int, int);
		Trigger(int, int, int, int, int, std::vector<Command*>&);
		Trigger(const Trigger& trigger);
		Trigger& operator=(const Trigger& trigger);
		~Trigger();
		int getEvent1() const;
		int getEvent2() const;
		int getEventType1() const;
		int getEventType2() const;
		int getOperator() const;
		std::vector<Command*>* getCommands();
		std::string toString() const;

		/// Member function that tests the trigger for a list of events.
		bool test(Memory& active, Memory& passive);

	protected:
		std::vector<Command*> commands;
		int event1, event2;
		int eventType1, eventType2;
		int optr;
	};
}

#endif


/**
 * \file SendUpCommand.hpp
 * \author Renato R. R. de Oliveira (renatorro@comp.ufla.br)
 * \author Ariel Marques
 * \version 1.0
 * \since Odin 1.0
 * \date 08/07/2012
 */

#ifndef GRUBI_ODIN_SENPUPCOMMANF_HPP
#define GRUBI_ODIN_SENPUPCOMMANF_HPP

#include <string>
#include <sstream>
#include "Command.hpp"
#include "Destination.hpp"
namespace odin {
	/**
	* .......
	*/
	class SendUpCommand : public Command{
	public:
		SendUpCommand(int eventNumber, Destination* destination);
		SendUpCommand(const SendUpCommand& command);
		~SendUpCommand();
		virtual void execute(Memory& active, Memory& passive, Connector * connector);
		virtual Command* clone();
		virtual int getEvent1() const { return this->eventNumber; }
		virtual int getEvent2() const { return 0; }
		virtual std::string toString() const;
	protected:
		int eventNumber;
		Destination* destination;
	};


}



#endif

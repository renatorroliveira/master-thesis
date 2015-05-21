/**
 * @file Executor.hpp
 * @author Renato R. R. de Oliveira (renatorro@comp.ufla.br)
 * @author Ariel Marques
 * @version 1.0
 * @since Odin 1.0
 * @date 09/07/2012
 */

#ifndef GRUBI_ODIN_EXECUTOR_HPP_INCLUDED
#define GRUBI_ODIN_EXECUTOR_HPP_INCLUDED

#include "Command.hpp"
#include "DestinationDown.hpp"
#include "DestinationLeft.hpp"
#include "DestinationRight.hpp"
#include "DestinationUp.hpp"
#include "Event.hpp"
#include "Memory.hpp"
#include "Connector.hpp"
#include "SendUpCommand.hpp"
#include "Trigger.hpp"
namespace odin{
	/**
		* Executor
		*/
	class Executor {
	public:
		Executor(std::vector<Trigger*>& list);
		~Executor();

		void execute(int source, Memory& active, Memory& passive, Connector* connector);
	protected:
		std::vector<Trigger*> commandList;
		bool firstTime;
	};

}
#endif

/**
 * @file Trigger.cpp
 * @author Renato R. R. de Oliveira (renatorro@comp.ufla.br)
 * @version 2.0
 * @since Odin 1.0
 * @date 13/08/2012
 */
#include "Trigger.hpp"
#include "../GMPI/Logger.hpp"

namespace odin {
	Trigger::Trigger(int ev1, int evt1, int ev2, int evt2, int op) {
		this->event1 = ev1;
		this->eventType1 = evt1;
		this->event2 = ev2;
		this->eventType2 = evt2;
		this->optr = op;
	}

	Trigger::Trigger(int ev1, int evt1, int ev2, int evt2, int op, std::vector<Command*>& cmds) : commands(cmds) {
		this->event1 = ev1;
		this->eventType1 = evt1;
		this->event2 = ev2;
		this->eventType2 = evt2;
		this->optr = op;
	}

	Trigger::Trigger(const Trigger& trigger) {
		(*this) = trigger;
	}

	Trigger& Trigger::operator=(const Trigger& trigger) {
		for (int c = 0; c < commands.size(); c++) {
			delete commands[c];
		}
		commands.clear();
		for (int c = 0; c < trigger.commands.size(); c++) {
			commands.push_back(trigger.commands[c]->clone());
		}
		this->event1 = trigger.event1;
		this->eventType1 = trigger.eventType1;
		this->event2 = trigger.event2;
		this->eventType2 = trigger.eventType2;
		this->optr = trigger.optr;
		return (*this);
	}

	Trigger::~Trigger() {
		for (int c = 0; c < commands.size(); c++) {
			delete commands[c];
		}
		commands.clear();
	}

	bool Trigger::test(Memory& active, Memory& passive) {
		Event* ev1 = this->eventType1 == 0 ? active[this->event1]:passive[this->event1];
		Event* ev2 = this->eventType2 == 0 ? active[this->event2]:passive[this->event2];
		switch (this->optr) {
			case 0:
				return (ev1->getFired() && ev2->getFired());
			case 1:
				return (ev1->getFired() || ev2->getFired());
			default:
				loggerpp::log("Trying to use an invalid operator ID.", loggerpp::level::FATAL_ERROR, this);
		}
	}

	int Trigger::getEvent1() const {
		return this->event1;
	}

	int Trigger::getEvent2() const {
		return this->event2;
	}
	int Trigger::getEventType1() const {
		return this->eventType1;
	}

	int Trigger::getEventType2() const {
		return this->eventType2;
	}

	int Trigger::getOperator() const {
		return this->optr;
	}

	std::vector<Command*>* Trigger::getCommands() {
		return (&commands);
	}

	std::string Trigger::toString() const {
		std::stringstream output;
		output << "if ("
				<< (this->eventType1 == 0 ? "active[":"passive[")
				<< this->event1 << "] "
				<< (this->optr == 1 ? "OR":"AND") << " "
				<< (this->eventType2 == 0 ? "active[":"passive[")
				<< this->event2 << "]) {" << std::endl;
		for (int c = 0; c < this->commands.size(); c++) {
			output << this->commands.at(c)->toString() << std::endl;
		}
		output << "}" << std::endl;
		return output.str();
	}
}

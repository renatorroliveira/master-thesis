/**
 * @file Solution.cpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since GeneticNet 1.0
 * @date 09/07/2012
 */

// Odin command implementations.
#include <Odin/Command.hpp>
#include <Odin/SendUpCommand.hpp>
#include <Odin/DownEventCommand.hpp>
#include <Odin/UpEventCommand.hpp>
#include <Odin/DestinationDown.hpp>
#include <Odin/DestinationLeft.hpp>
#include <Odin/DestinationRight.hpp>
#include <Odin/DestinationUp.hpp>
#include <Odin/Trigger.hpp>

#include <boost/random.hpp>

#include <cmath>
#include <cstdlib>
#include <typeinfo>

#include "Solution.hpp"

using namespace odin;
using namespace boost;

namespace geneticnet {
	Solution::Solution(tinynode::Simulator* simulator) {
		this->simulator = simulator;
		
		this->maximumSize = 3;
		this->triggerLimit = 10 * this->simulator->getSetup()->nodesPassiveMemorySize;
#ifndef NO_SPLIT_EVENTS
		this->numberOfEventTypes = this->simulator->getNumberOfEvents();
#else
		this->numberOfEventTypes = 1;
#endif
		//std::cout << "Numero de divisoes na codificacao: " << this->numberOfEventTypes << std::endl;
		this->numberOfActiveEvents = this->simulator->getSetup()->nodesActiveMemorySize;
		this->numberOfPassiveEvents = this->simulator->getSetup()->nodesPassiveMemorySize;
		this->totalEvents = this->numberOfActiveEvents+this->numberOfPassiveEvents;
		
		this->fitness = 0.0f;
		
		this->program = new std::vector<Trigger*>[this->numberOfEventTypes];
		for (int t = 0; t < this->numberOfEventTypes; t++)
			this->program[t].reserve(this->triggerLimit);
		
		this->rng.seed(std::time(0));
		
		this->initialize();
	}
	
	Solution::Solution(const Solution& sol) {
		(*this) = sol;
		this->rng.seed(((int)std::clock())+std::rand());
	}
	
	Solution& Solution::operator=(const Solution& sol) {
		this->simulator = sol.simulator;
		this->maximumSize = sol.maximumSize;
		this->triggerLimit = sol.triggerLimit;
		this->numberOfActiveEvents = sol.numberOfActiveEvents;
		this->numberOfPassiveEvents = sol.numberOfPassiveEvents;
		this->totalEvents = sol.numberOfActiveEvents+sol.numberOfPassiveEvents;
		
		this->fitness = sol.fitness;
		this->fitness_ps = sol.fitness_ps;
		this->fitness_psafs = sol.fitness_psafs;
		this->fitness_psbat = sol.fitness_psbat;
		this->fitness_eabat = sol.fitness_eabat;
		this->fitness_tnras = sol.fitness_tnras;
		this->fitness_tmdts = sol.fitness_tmdts;
		
		if (this->program != 0) {
			for (int t = 0; t < this->numberOfEventTypes; t++) {
				for (int cmd = 0; cmd < this->program[t].size(); cmd++) {
					delete this->program[t][cmd];
				}
				this->program[t].clear();
				this->program[t].reserve(this->maximumSize);
			}
			if (this->numberOfEventTypes != sol.numberOfEventTypes) {
				this->numberOfEventTypes = sol.numberOfEventTypes;
				delete [] this->program;
				this->program = new std::vector<Trigger*>[this->numberOfEventTypes];
			}
		} else {
			this->numberOfEventTypes = sol.numberOfEventTypes;
			this->program = new std::vector<Trigger*>[this->numberOfEventTypes];
		}
		for (int t = 0; t < sol.numberOfEventTypes; t++) {
			for (int cmd = 0; cmd < sol.program[t].size(); cmd++) {
				this->program[t].push_back(new odin::Trigger(*(sol.program[t][cmd])));
			}
		}
		
		return (*this);
	}
	
	Solution::~Solution() {
		for (int t = 0; t < this->numberOfEventTypes; t++) {
			for (int i = 0; i < this->program[t].size(); i++) {
				delete this->program[t][i];
			}
			this->program[t].clear();
		}
		delete [] this->program;
	}
	
	void Solution::initialize() {
		for (int t = 0; t < this->numberOfEventTypes; t++) {
			this->generateRandomCommands(this->program[t]);
		}
	}
	
	void Solution::generateRandomCommands(std::vector<odin::Trigger*>& cmds) {
		int newSize = 1 + (std::rand() % (this->triggerLimit-1));
		for (int i = 0; i < cmds.size(); i++) {
			delete cmds[i];
		}
		cmds.clear();
		int nCommands = 0;
		for (int i = 0; i < newSize; i++) {
			odin::Trigger* trigger = this->generateRandomTrigger();
			nCommands = 1+(std::rand() % (this->maximumSize-1));
			for (int j = 0; j < nCommands; j++) {
				trigger->getCommands()->push_back(this->generateRandomCommand());
			}
			cmds.push_back(trigger);
		}
	}
	
	odin::Command* Solution::generateRandomCommand() {
		int cmdType = std::rand()%3;
		int ev1 = 0,
			destination = 0
		;
		switch (cmdType) {
			case 0: // DownEvent
				ev1 = rand()%this->numberOfPassiveEvents;
				return new DownEventCommand(ev1);
			case 1: // UpEvent
				ev1 = rand()%this->numberOfPassiveEvents;
				return new UpEventCommand(ev1);
			case 2: // SendUPEvent
				ev1 = rand()%this->numberOfPassiveEvents;
				destination = rand()%4;
				if (destination == 0)
					return new SendUpCommand(ev1, new DestinationDown());
				else if (destination == 1)
					return new SendUpCommand(ev1, new DestinationLeft());
				else if (destination == 2)
					return new SendUpCommand(ev1, new DestinationRight());
				else if (destination == 3)
					return new SendUpCommand(ev1, new DestinationUp());
			default:
				return NULL;
		}
	}
	
	odin::Trigger* Solution::generateRandomTrigger() {
		int ev1 = 0,
			ev1T = 0,
			ev2 = 0,
			ev2T = 0,
			optr = 0
		;
		ev1T = std::rand()%2;
		ev2T = std::rand()%2;
		if (ev1T == 0) {
			ev1 = std::rand()%this->numberOfActiveEvents;
		} else {
			ev1 = std::rand()%this->numberOfPassiveEvents;
		}
		if (ev2T == 0) {
			ev2 = std::rand()%this->numberOfActiveEvents;
		} else {
			ev2 = std::rand()%this->numberOfPassiveEvents;
		}
		optr = std::rand()%2;
		return new Trigger(ev1, ev1T, ev2, ev2T, optr);
	}
	
	double Solution::calculateFitness() {
		/*
		 * Este é o método que realiza a chamada ao GRUBIX.
		 */
		this->repair();
		SensorNetworkStatistics statistics;
		int totalSize = 0;
		if (this->numberOfEventTypes > 1) {
			std::vector<Trigger*> fullProgram;
			for (int t = 0; t < this->numberOfEventTypes; t++)
				totalSize += this->program[t].size();
			fullProgram.reserve(totalSize);
			this->fitness = 0.0f;
			for (int t = 0; t < this->numberOfEventTypes; t++) {
				for (int trig = 0; trig < this->program[t].size(); trig++) {
					this->fitness += this->program[t][trig]->getCommands()->size();
					fullProgram.push_back(this->program[t][trig]);
				}
			}
			this->simulator->simulate(&statistics, fullProgram);
		} else {
			this->fitness = 0.0f;
			for (int trig = 0; trig < this->program[0].size(); trig++) {
				this->fitness += this->program[0][trig]->getCommands()->size();
			}
			totalSize = this->program[0].size();
			this->simulator->simulate(&statistics, this->program[0]);
		}
		statistics.postProcessStatistics();
		
		this->fitness += this->fitness_ps = 2.0f*statistics.packets_sent;
		this->fitness += this->fitness_eabat = 2500.0f * statistics.events_active_before_any_trigger;
		this->fitness += this->fitness_tnras = 5000.0f * statistics.triggers_not_reached_any_sink;
		this->fitness += this->fitness_tmdts = 500.0f * std::pow(statistics.triggers_min_dist_to_sink, 2.0f);
		this->fitness += this->fitness_psbat = 500.0f * statistics.packets_sent_before_any_trigger;
		this->fitness += this->fitness_psafs = 500.0f * statistics.packets_sent_away_from_sink * statistics.triggers_not_reached_any_sink;
		return this->fitness;
	}
	
	void Solution::generateBestTrivial() {
		for (int evt = 0; evt < this->numberOfEventTypes; evt++) {
			for (int c = 0; c < this->program[evt].size(); c++) {
				delete this->program[evt][c];
			}
			this->program[evt].clear();
		}
		odin::Trigger* t;
		int base = this->simulator->getSetup()->nodesActiveMemorySize - 4;
		for (int evt = 0; evt < this->numberOfEventTypes; evt++) {
			t = new Trigger(evt, 1, base, 0, 0);
			this->program[evt].push_back(t);
			t->getCommands()->push_back(new SendUpCommand(evt, new DestinationUp()));
			t = new Trigger(evt, 1, base+1, 0, 0);
			this->program[evt].push_back(t);
			t->getCommands()->push_back(new SendUpCommand(evt, new DestinationRight()));
			t = new Trigger(evt, 1, base+2, 0, 0);
			this->program[evt].push_back(t);
			t->getCommands()->push_back(new SendUpCommand(evt, new DestinationDown()));
			t = new Trigger(evt, 1, base+3, 0, 0);
			this->program[evt].push_back(t);
			t->getCommands()->push_back(new SendUpCommand(evt, new DestinationLeft()));
			t = new Trigger(evt, 0, evt, 0, 1);
			this->program[evt].push_back(t);
			t->getCommands()->push_back(new UpEventCommand(evt));
		}
	}
	
	void Solution::repair() {
		for (int t = 0; t < this->numberOfEventTypes; t++) {
			int c = 0;
			while (c < this->program[t].size()) {
				if (this->program[t][c]->getCommands()->size() <= 0) {
					delete this->program[t][c];
					this->program[t].erase(this->program[t].begin()+c);
				} else {
					c++;
				}
			}
		}
	}
	
	double Solution::getFitness() const {
		return this->fitness;
	}
	
	void Solution::writeTextProgram(std::ostream& output) {
		this->calculateFitness();
		int progSize = 0;
		for (int t = 0; t < this->numberOfEventTypes; t++) {
			for (int trig = 0; trig < this->program[t].size(); trig++) {
				progSize += this->program[t][trig]->getCommands()->size();
			}
		}
		output << "====================\n";
		output << "Fitness Program Size: " << progSize << "\n";
		output << "Fitness Packets Sent: " << this->fitness_ps << "\n";
		output << "Fitness Packets Sent Away from Sink: " << this->fitness_psafs << "\n";
		output << "Fitness Packets Sent Before any Trigger: " << this->fitness_psbat << "\n";
		output << "Fitness Event Active Before any Trigger: " << this->fitness_eabat << "\n";
		output << "Fitness Triggers not Reached any Sink: " << this->fitness_tnras << "\n";
		output << "Fitness Triggers Min Distance to Sink: " << this->fitness_tmdts << "\n";
		output << "Total Fitness: " << this->fitness << "\n";
		output << "====================\n\n";
		for (int t = 0; t < this->numberOfEventTypes; t++) {
			for (int trig = 0; trig < this->program[t].size(); trig++) {
				output << this->program[t].at(trig)->toString();
			}
		}
		output.flush();
	}
}


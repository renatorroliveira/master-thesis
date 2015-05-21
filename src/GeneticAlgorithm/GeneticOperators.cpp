/**
 * @file GeneticOperators.hpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since GeneticNet 1.0
 * @date 09/07/2012
 */

#include <vector>
#include <boost/random.hpp>
#include <Odin/Command.hpp>

#include "GeneticOperators.hpp"
#include "Solution.hpp"

using namespace boost::random;
using odin::Command;
using odin::Trigger;

namespace geneticnet {
	namespace operators {
		/**
		 * This is just a container to make all implementations of operators to
		 * be friends of solution and then gain access to non-public content of
		 * Solution objects. Any new operator implementation should be declared
		 * inside this class as a private static method.
		 */
		class impl {
			friend void geneticnet::operators::recombine(Solution* parent1, Solution* parent2, Solution* child);
			friend void geneticnet::operators::recombine(Solution* parent1, Solution* parent2, Solution* child, int type);
			friend void geneticnet::operators::mutate(Solution* solution);
			friend void geneticnet::operators::mutate(Solution* solution, int type);
		private:
			static void __crossover_trigger_uniform(std::vector<Trigger*>& parent1, std::vector<Trigger*>& parent2, std::vector<Trigger*>& child);
			static void __crossover_command_uniform(std::vector<Trigger*>& parent1, std::vector<Trigger*>& parent2, std::vector<Trigger*>& child);
			static void __crossover_trigger_onepoint(std::vector<Trigger*>& parent1, std::vector<Trigger*>& parent2, std::vector<Trigger*>& child);
			static void __crossover_command_onepoint(std::vector<Trigger*>& parent1, std::vector<Trigger*>& parent2, std::vector<Trigger*>& child);
			
			static void __mutation_insert_random_command(Solution* sol);
			static void __mutation_remove_random_command(Solution* sol);
			static void __mutation_swap_random_command(Solution* sol);
			static void __mutation_change_trigger_events(Solution* sol);
			static void __mutation_insert_random_trigger(Solution* sol);
			static void __mutation_remove_random_trigger(Solution* sol);
		};
		
		mt19937 rng;
		uniform_int_distribution<> intrand;
		
		void recombine(Solution* parent1, Solution* parent2, Solution* child, int type) {
			switch (type) {
				case 0:
					for (int t = 0; t < child->numberOfEventTypes; t++)
						impl::__crossover_trigger_uniform(parent1->program[t], parent2->program[t], child->program[t]);
					break;
				case 1:
					for (int t = 0; t < child->numberOfEventTypes; t++)
						impl::__crossover_command_uniform(parent1->program[t], parent2->program[t], child->program[t]);
					break;
				case 2:
					for (int t = 0; t < child->numberOfEventTypes; t++)
						impl::__crossover_trigger_onepoint(parent1->program[t], parent2->program[t], child->program[t]);
					break;
				case 3:
					for (int t = 0; t < child->numberOfEventTypes; t++)
						impl::__crossover_command_onepoint(parent1->program[t], parent2->program[t], child->program[t]);
					break;
				default:
					loggerpp::log("Invalid crossover type provided.", loggerpp::level::FATAL_ERROR);
					break;
			}
		}
		
		void recombine(Solution* parent1, Solution* parent2, Solution* child) {
			int recombination = intrand(rng) % 4;
			recombine(parent1, parent2, child, recombination);
		}
		
		void mutate(Solution* solution, int type) {
			switch (type) {
				case 0:
					impl::__mutation_insert_random_command(solution);
					break;
				case 1:
					impl::__mutation_remove_random_command(solution);
					break;
				case 2:
					impl::__mutation_swap_random_command(solution);
					break;
				case 3:
					impl::__mutation_change_trigger_events(solution);
					break;
				case 4:
					impl::__mutation_insert_random_trigger(solution);
					break;
				case 5:
					impl::__mutation_remove_random_trigger(solution);
					break;
				default:
					loggerpp::log("Invalid mutation type provided.", loggerpp::level::FATAL_ERROR);
					break;
			}
		}
		
		void mutate(Solution* solution) {
			int mutation = intrand(rng) % 5;
			mutate(solution, mutation);
		}
		
		
		
		/*
		 * RECOMBINATIONS IMPLEMENTATIONS
		 */
		void impl::__crossover_trigger_uniform(std::vector<Trigger*>& parent1, std::vector<Trigger*>& parent2, std::vector<Trigger*>& child) {
			for (int c = 0; c < child.size(); c++) {
				delete child[c];
			}
			child.clear();
			if (parent1.size() <= parent2.size()) {
				for (int c = 0; c < parent1.size(); c++) {
					if (intrand(rng) % 2 == 0) {
						child.push_back(new odin::Trigger(*(parent1[c])));
					} else {
						child.push_back(new odin::Trigger(*(parent2[c])));
					}
				}
				for (int c = parent1.size(); c < parent2.size(); c++) {
					if (intrand(rng) % 2 == 0) {
						child.push_back(new odin::Trigger(*(parent2[c])));
					}
				}
			} else {
				for (int c = 0; c < parent2.size(); c++) {
					if (intrand(rng) % 2 == 0) {
						child.push_back(new odin::Trigger(*(parent1[c])));
					} else {
						child.push_back(new odin::Trigger(*(parent2[c])));
					}
				}
				for (int c = parent2.size(); c < parent1.size(); c++) {
					if (intrand(rng) % 2 == 0) {
						child.push_back(new odin::Trigger(*(parent1[c])));
					}
				}
			}
		}
		
		void impl::__crossover_command_uniform(std::vector<Trigger*>& parent1, std::vector<Trigger*>& parent2, std::vector<Trigger*>& child) {
			for (int c = 0; c < child.size(); c++) {
				delete child[c];
			}
			child.clear();
			odin::Trigger *trig;
			if (parent1.size() <= parent2.size()) {
				for (int t = 0; t < parent1.size(); t++) {
					if (intrand(rng) % 2 == 0) {
						trig = new odin::Trigger(
							parent1[t]->getEvent1(), parent1[t]->getEventType1(),
							parent1[t]->getEvent2(), parent1[t]->getEventType2(),
							parent1[t]->getOperator()
						);
					} else {
						trig = new odin::Trigger(
							parent2[t]->getEvent1(), parent2[t]->getEventType1(),
							parent2[t]->getEvent2(), parent2[t]->getEventType2(),
							parent2[t]->getOperator()
						);
					}
					if (parent1[t]->getCommands()->size() <= parent2[t]->getCommands()->size()) {
						for (int c = 0; c < parent1[t]->getCommands()->size(); c++) {
							if (intrand(rng) % 2 == 0) {
								trig->getCommands()->push_back(parent1[t]->getCommands()->at(c)->clone());
							} else {
								trig->getCommands()->push_back(parent2[t]->getCommands()->at(c)->clone());
							}
						}
						for (int c = parent1[t]->getCommands()->size(); c < parent2[t]->getCommands()->size(); c++) {
							if (intrand(rng) % 2 == 0) {
								trig->getCommands()->push_back(parent2[t]->getCommands()->at(c)->clone());
							}
						}
					} else {
						for (int c = 0; c < parent2[t]->getCommands()->size(); c++) {
							if (intrand(rng) % 2 == 0) {
								trig->getCommands()->push_back(parent1[t]->getCommands()->at(c)->clone());
							} else {
								trig->getCommands()->push_back(parent2[t]->getCommands()->at(c)->clone());
							}
						}
						for (int c = parent2[t]->getCommands()->size(); c < parent1[t]->getCommands()->size(); c++) {
							if (intrand(rng) % 2 == 0) {
								trig->getCommands()->push_back(parent1[t]->getCommands()->at(c)->clone());
							}
						}
					}
					child.push_back(trig);
				}
				for (int t = parent1.size(); t < parent2.size(); t++) {
					if (intrand(rng) % 2 == 0) {
						trig = new odin::Trigger(
							parent2[t]->getEvent1(), parent2[t]->getEventType1(),
							parent2[t]->getEvent2(), parent2[t]->getEventType2(),
							parent2[t]->getOperator()
						);
						for (int c = 0; c < parent2[t]->getCommands()->size(); c++) {
							if (intrand(rng) % 2 == 0) {
								trig->getCommands()->push_back(parent2[t]->getCommands()->at(c)->clone());
							}
						}
						if (trig->getCommands()->size() > 0)
							child.push_back(trig);
					}
				}
			} else {
				for (int t = 0; t < parent2.size(); t++) {
					if (intrand(rng) % 2 == 0) {
						trig = new odin::Trigger(
							parent1[t]->getEvent1(), parent1[t]->getEventType1(),
							parent1[t]->getEvent2(), parent1[t]->getEventType2(),
							parent1[t]->getOperator()
						);
					} else {
						trig = new odin::Trigger(
							parent2[t]->getEvent1(), parent2[t]->getEventType1(),
							parent2[t]->getEvent2(), parent2[t]->getEventType2(),
							parent2[t]->getOperator()
						);
					}
					if (parent1[t]->getCommands()->size() <= parent2[t]->getCommands()->size()) {
						for (int c = 0; c < parent1[t]->getCommands()->size(); c++) {
							if (intrand(rng) % 2 == 0) {
								trig->getCommands()->push_back(parent1[t]->getCommands()->at(c)->clone());
							} else {
								trig->getCommands()->push_back(parent2[t]->getCommands()->at(c)->clone());
							}
						}
						for (int c = parent1[t]->getCommands()->size(); c < parent2[t]->getCommands()->size(); c++) {
							if (intrand(rng) % 2 == 0) {
								trig->getCommands()->push_back(parent2[t]->getCommands()->at(c)->clone());
							}
						}
					} else {
						for (int c = 0; c < parent2[t]->getCommands()->size(); c++) {
							if (intrand(rng) % 2 == 0) {
								trig->getCommands()->push_back(parent1[t]->getCommands()->at(c)->clone());
							} else {
								trig->getCommands()->push_back(parent2[t]->getCommands()->at(c)->clone());
							}
						}
						for (int c = parent2[t]->getCommands()->size(); c < parent1[t]->getCommands()->size(); c++) {
							if (intrand(rng) % 2 == 0) {
								trig->getCommands()->push_back(parent1[t]->getCommands()->at(c)->clone());
							}
						}
					}
					child.push_back(trig);
				}
				for (int t = parent2.size(); t < parent1.size(); t++) {
					if (intrand(rng) % 2 == 0) {
						trig = new odin::Trigger(
							parent1[t]->getEvent1(), parent1[t]->getEventType1(),
							parent1[t]->getEvent2(), parent1[t]->getEventType2(),
							parent1[t]->getOperator()
						);
						for (int c = 0; c < parent1[t]->getCommands()->size(); c++) {
							if (intrand(rng) % 2 == 0) {
								trig->getCommands()->push_back(parent1[t]->getCommands()->at(c)->clone());
							}
						}
						if (trig->getCommands()->size() > 0)
							child.push_back(trig);
					}
				}
			}
		}
		
		void impl::__crossover_trigger_onepoint(std::vector<Trigger*>& parent1, std::vector<Trigger*>& parent2, std::vector<Trigger*>& child) {
			for (int c = 0; c < child.size(); c++) {
				delete child[c];
			}
			child.clear();
			if (parent1.size() <= parent2.size()) {
				int point = intrand(rng) % parent1.size();
				if (intrand(rng) % 2 == 0) {
					for (int c = 0; c <= point; c++) {
						child.push_back(new odin::Trigger(*(parent1[c])));
					}
					for (int c = point+1; c < parent2.size(); c++) {
						child.push_back(new odin::Trigger(*(parent2[c])));
					}
				} else {
					for (int c = 0; c <= point; c++) {
						child.push_back(new odin::Trigger(*(parent2[c])));
					}
					for (int c = point+1; c < parent1.size(); c++) {
						child.push_back(new odin::Trigger(*(parent1[c])));
					}
				}
			} else {
				int point = intrand(rng) % parent2.size();
				if (intrand(rng) % 2 == 0) {
					for (int c = 0; c <= point; c++) {
						child.push_back(new odin::Trigger(*(parent1[c])));
					}
					for (int c = point+1; c < parent2.size(); c++) {
						child.push_back(new odin::Trigger(*(parent2[c])));
					}
				} else {
					for (int c = 0; c <= point; c++) {
						child.push_back(new odin::Trigger(*(parent2[c])));
					}
					for (int c = point+1; c < parent1.size(); c++) {
						child.push_back(new odin::Trigger(*(parent1[c])));
					}
				}
			}
		}
		
		void impl::__crossover_command_onepoint(std::vector<Trigger*>& parent1, std::vector<Trigger*>& parent2, std::vector<Trigger*>& child) {
			for (int c = 0; c < child.size(); c++) {
				delete child[c];
			}
			child.clear();
			odin::Trigger *trig;
			if (parent1.size() <= parent2.size()) {
				for (int t = 0; t < parent1.size(); t++) {
					if (intrand(rng) % 2 == 0) {
						trig = new odin::Trigger(
							parent1[t]->getEvent1(), parent1[t]->getEventType1(),
							parent1[t]->getEvent2(), parent1[t]->getEventType2(),
							parent1[t]->getOperator()
						);
					} else {
						trig = new odin::Trigger(
							parent2[t]->getEvent1(), parent2[t]->getEventType1(),
							parent2[t]->getEvent2(), parent2[t]->getEventType2(),
							parent2[t]->getOperator()
						);
					}
					if (parent1[t]->getCommands()->size() <= parent2[t]->getCommands()->size()) {
						int point = intrand(rng) % parent1[t]->getCommands()->size();
						if (intrand(rng) % 2 == 0) {
							for (int c = 0; c <= point; c++) {
								trig->getCommands()->push_back(parent1[t]->getCommands()->at(c)->clone());
							}
							for (int c = point + 1; c < parent2[t]->getCommands()->size(); c++) {
								trig->getCommands()->push_back(parent2[t]->getCommands()->at(c)->clone());
							}
						} else {
							for (int c = 0; c <= point; c++) {
								trig->getCommands()->push_back(parent2[t]->getCommands()->at(c)->clone());
							}
							for (int c = point + 1; c < parent1[t]->getCommands()->size(); c++) {
								trig->getCommands()->push_back(parent1[t]->getCommands()->at(c)->clone());
							}
						}
					} else {
						int point = intrand(rng) % parent2[t]->getCommands()->size();
						if (intrand(rng) % 2 == 0) {
							for (int c = 0; c <= point; c++) {
								trig->getCommands()->push_back(parent1[t]->getCommands()->at(c)->clone());
							}
							for (int c = point + 1; c < parent2[t]->getCommands()->size(); c++) {
								trig->getCommands()->push_back(parent2[t]->getCommands()->at(c)->clone());
							}
						} else {
							for (int c = 0; c <= point; c++) {
								trig->getCommands()->push_back(parent2[t]->getCommands()->at(c)->clone());
							}
							for (int c = point + 1; c < parent1[t]->getCommands()->size(); c++) {
								trig->getCommands()->push_back(parent1[t]->getCommands()->at(c)->clone());
							}
						}
					}
					child.push_back(trig);
				}
				for (int t = parent1.size(); t < parent2.size(); t++) {
					if (intrand(rng) % 2 == 0) {
						trig = new odin::Trigger(
							parent2[t]->getEvent1(), parent2[t]->getEventType1(),
							parent2[t]->getEvent2(), parent2[t]->getEventType2(),
							parent2[t]->getOperator()
						);
						for (int c = 0; c < parent2[t]->getCommands()->size(); c++) {
							if (intrand(rng) % 2 == 0) {
								trig->getCommands()->push_back(parent2[t]->getCommands()->at(c)->clone());
							}
						}
						if (trig->getCommands()->size() > 0)
							child.push_back(trig);
					}
				}
			} else {
				for (int t = 0; t < parent2.size(); t++) {
					if (intrand(rng) % 2 == 0) {
						trig = new odin::Trigger(
							parent1[t]->getEvent1(), parent1[t]->getEventType1(),
							parent1[t]->getEvent2(), parent1[t]->getEventType2(),
							parent1[t]->getOperator()
						);
					} else {
						trig = new odin::Trigger(
							parent2[t]->getEvent1(), parent2[t]->getEventType1(),
							parent2[t]->getEvent2(), parent2[t]->getEventType2(),
							parent2[t]->getOperator()
						);
					}
					if (parent1[t]->getCommands()->size() <= parent2[t]->getCommands()->size()) {
						int point = intrand(rng) % parent1[t]->getCommands()->size();
						if (intrand(rng) % 2 == 0) {
							for (int c = 0; c <= point; c++) {
								trig->getCommands()->push_back(parent1[t]->getCommands()->at(c)->clone());
							}
							for (int c = point + 1; c < parent2[t]->getCommands()->size(); c++) {
								trig->getCommands()->push_back(parent2[t]->getCommands()->at(c)->clone());
							}
						} else {
							for (int c = 0; c <= point; c++) {
								trig->getCommands()->push_back(parent2[t]->getCommands()->at(c)->clone());
							}
							for (int c = point + 1; c < parent1[t]->getCommands()->size(); c++) {
								trig->getCommands()->push_back(parent1[t]->getCommands()->at(c)->clone());
							}
						}
					} else {
						int point = intrand(rng) % parent2[t]->getCommands()->size();
						if (intrand(rng) % 2 == 0) {
							for (int c = 0; c <= point; c++) {
								trig->getCommands()->push_back(parent1[t]->getCommands()->at(c)->clone());
							}
							for (int c = point + 1; c < parent2[t]->getCommands()->size(); c++) {
								trig->getCommands()->push_back(parent2[t]->getCommands()->at(c)->clone());
							}
						} else {
							for (int c = 0; c <= point; c++) {
								trig->getCommands()->push_back(parent2[t]->getCommands()->at(c)->clone());
							}
							for (int c = point + 1; c < parent1[t]->getCommands()->size(); c++) {
								trig->getCommands()->push_back(parent1[t]->getCommands()->at(c)->clone());
							}
						}
					}
					child.push_back(trig);
				}
				for (int t = parent2.size(); t < parent1.size(); t++) {
					if (intrand(rng) % 2 == 0) {
						trig = new odin::Trigger(
							parent1[t]->getEvent1(), parent1[t]->getEventType1(),
							parent1[t]->getEvent2(), parent1[t]->getEventType2(),
							parent1[t]->getOperator()
						);
						for (int c = 0; c < parent1[t]->getCommands()->size(); c++) {
							if (intrand(rng) % 2 == 0) {
								trig->getCommands()->push_back(parent1[t]->getCommands()->at(c)->clone());
							}
						}
						if (trig->getCommands()->size() > 0)
							child.push_back(trig);
					}
				}
			}
		}
		
		
		
		/*
		 * MUTATIONS IMPLEMENTATIONS
		 */
		void impl::__mutation_insert_random_command(Solution* sol) {
			int evt = intrand(rng) % sol->numberOfEventTypes;
			Command* cmd = sol->generateRandomCommand();
			if (sol->program[evt].size() == 0) {
				sol->program[evt].push_back(sol->generateRandomTrigger());
			}
			int pos = intrand(rng) % sol->program[evt].size();
			if (sol->program[evt].at(pos)->getCommands()->size() >= sol->maximumSize) {
				Trigger* trigger = sol->generateRandomTrigger();
				trigger->getCommands()->push_back(cmd);
				sol->program[evt].insert(sol->program[evt].begin()+pos, trigger);
			} else {
				sol->program[evt].at(pos)->getCommands()->insert(
					sol->program[evt].at(pos)->getCommands()->begin()+(intrand(rng)%sol->program[evt].at(pos)->getCommands()->size()), cmd);
			}
		}
		
		void impl::__mutation_remove_random_command(Solution* sol) {
			int evt = intrand(rng) % sol->numberOfEventTypes;
			if (sol->program[evt].size() < 1)
				return;
			if ((sol->program[evt].size() == 1) && (sol->program[evt].at(0)->getCommands()->size() <= 1))
				return;
			int pos = intrand(rng) % sol->program[evt].size();
			int poscmd = intrand(rng) % sol->program[evt].at(pos)->getCommands()->size();
			delete sol->program[evt].at(pos)->getCommands()->at(poscmd);
			sol->program[evt].at(pos)->getCommands()->erase(sol->program[evt].at(pos)->getCommands()->begin()+poscmd);
		}
		
		void impl::__mutation_swap_random_command(Solution* sol) {
			int evt = intrand(rng) % sol->numberOfEventTypes;
			if (sol->program[evt].size() <= 0) {
				impl::__mutation_insert_random_command(sol);
				return;
			}
			if ((sol->program[evt].size() == 1) && (sol->program[evt].at(0)->getCommands()->size() <= 0))
				return;
			Command* cmd = sol->generateRandomCommand();
			int pos = intrand(rng) % sol->program[evt].size();
			int poscmd = intrand(rng) % sol->program[evt].at(pos)->getCommands()->size();
			delete sol->program[evt].at(pos)->getCommands()->at(poscmd);
			sol->program[evt].at(pos)->getCommands()->at(poscmd) = cmd;
		}
		
		void impl::__mutation_change_trigger_events(Solution* sol) {
			int evt = intrand(rng) % sol->numberOfEventTypes;
			if (sol->program[evt].size() == 0) {
				__mutation_insert_random_trigger(sol);
				return;
			}
			int pos = intrand(rng) % sol->program[evt].size();
			std::vector<Command*>* cmds = sol->program[evt].at(pos)->getCommands();
			Trigger* trigger = sol->generateRandomTrigger();
			for (int c = 0 ; c < cmds->size(); c++) {
				trigger->getCommands()->push_back(cmds->at(c)->clone());
			}
			delete sol->program[evt].at(pos);
			sol->program[evt].at(pos) = trigger;
		}
		
		void impl::__mutation_insert_random_trigger(Solution* sol) {
			int evt = intrand(rng) % sol->numberOfEventTypes;
			if (sol->program[evt].size() >= sol->triggerLimit) {
				__mutation_remove_random_trigger(sol);
				return;
			}
			Trigger* t = sol->generateRandomTrigger();
			t->getCommands()->push_back(sol->generateRandomCommand());
			sol->program[evt].push_back(t);
		}
		
		void impl::__mutation_remove_random_trigger(Solution* sol) {
			int evt = intrand(rng) % sol->numberOfEventTypes;
			if (sol->program[evt].size() == 0) {
				__mutation_insert_random_trigger(sol);
				return;
			}
			if (sol->program[evt].size() == 1) {
				__mutation_change_trigger_events(sol);
				return;
			}
			int pos = intrand(rng) % sol->program[evt].size();
			delete sol->program[evt].at(pos);
			sol->program[evt].erase(sol->program[evt].begin() + pos);
		}
	}
}

/**
 * @file Solution.hpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 2.0
 * @since GeneticNet 1.0
 * @date 13/08/2012
 */

#ifndef GRUBI_GENETICNET_SOLUTION_HPP_INCLUDED
#define GRUBI_GENETICNET_SOLUTION_HPP_INCLUDED

#include <ctime>
#include <vector>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <Odin/Trigger.hpp>
#include <Odin/Command.hpp>
#include <TinyNode/Simulator.hpp>

#include "GeneticOperators.hpp"
#include "SensorNetworkStatistics.hpp"

namespace geneticnet {
	class SensorNetwork;
	class Solution {
		friend void geneticnet::operators::recombine(Solution* parent1, Solution* parent2, Solution* child);
		friend void geneticnet::operators::recombine(Solution* parent1, Solution* parent2, Solution* child, int type);
		friend void geneticnet::operators::mutate(Solution* solution);
		friend void geneticnet::operators::mutate(Solution* solution, int type);
		friend class geneticnet::operators::impl;
		friend class geneticnet::SensorNetwork;
		friend class boost::serialization::access;
	public:
		boost::random::mt19937 rng;
		Solution(tinynode::Simulator* simulator);
		Solution(const Solution& sol);
		Solution& operator=(const Solution& sol);
		~Solution();
	
		/// Randomly initializes the solution.
		void initialize();
		/// Generates a random set of commands.
		void generateRandomCommands(std::vector<odin::Trigger*>& cmds);
		/// Generates a random command.
		odin::Command* generateRandomCommand();
		/// Generates a random trigger.
		odin::Trigger* generateRandomTrigger();
		/// Calcula o fitness da solução atual.
		double calculateFitness();
		/// Returns the already calculated fitness.
		double getFitness() const;
		/// Routine that repairs useless information in the program.
		void repair();
		/// Generates the best trivial solution.
		void generateBestTrivial();
		
		/// Write a human-friendly text version of the program.
		void writeTextProgram(std::ostream& output);
	protected:
		tinynode::Simulator* simulator;
		std::vector<odin::Trigger*>* program;
		double fitness;
		double fitness_tnras,
			fitness_tmdts,
			fitness_ps,
			fitness_psafs,
			fitness_psbat,
			fitness_eabat;
		
		int maximumSize;
		int triggerLimit;
		int numberOfActiveEvents;
		int numberOfPassiveEvents;
		int numberOfEventTypes;
		int totalEvents;
		
		/// Solution serialization.
		template<class Archive>
		void serialize(Archive & archive, const unsigned int version) {
			archive & BOOST_SERIALIZATION_NVP(fitness);
        }
	};
}

#endif

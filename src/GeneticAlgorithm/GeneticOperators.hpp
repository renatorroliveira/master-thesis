/**
 * @file GeneticOperators.hpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since GeneticNet 1.0
 * @date 09/07/2012
 */

#ifndef GRUBI_GENETICNET_GENETICOPERATORS_HPP_INCLUDED
#define GRUBI_GENETICNET_GENETICOPERATORS_HPP_INCLUDED

namespace geneticnet {
	class Solution;
	namespace operators {
		void recombine(Solution* parent1, Solution* parent2, Solution* child);
		void recombine(Solution* parent1, Solution* parent2, Solution* child, int type);
		void mutate(Solution* solution);
		void mutate(Solution* solution, int type);
		class impl;
	}
}

#endif

/**
 * @file AbstractMetaheuristic.hpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since GeneticNet 1.0
 * @date 10/07/2012
 */

#ifndef GRUBI_GENETICNET_ABSTRACTMETAHEURISTIC_HPP_INCLUDED
#define GRUBI_GENETICNET_ABSTRACTMETAHEURISTIC_HPP_INCLUDED

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include "ResultManager.hpp"

namespace geneticnet {
	class AbstractMetaheuristic {
	public:
		virtual ~AbstractMetaheuristic() {}
		virtual void execute(ResultManager * results) = 0;
		virtual void save_setup_to_xml(boost::archive::xml_oarchive& output) = 0;
		virtual void load_setup_from_xml(boost::archive::xml_iarchive& input) = 0;
		static AbstractMetaheuristic* for_metaheuristic_id(const int id);
	};
}

#endif

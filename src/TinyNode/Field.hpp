/**
 * @file Field.hpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since TinyNode 1.0
 * @date 13/07/2012
 */

#ifndef GRUBI_TINYNODE_FIELD_HPP_INCLUDED
#define GRUBI_TINYNODE_FIELD_HPP_INCLUDED

#include <string>
#include <sstream>
#include <cstdlib>
#include <vector>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/version.hpp>

#include <GMPI/Logger.hpp>

#include "Position.hpp"
#include "SensorNode.hpp"
#include "OdinExecutor.hpp"

namespace tinynode {
	class Field {
		friend class boost::serialization::access;
	protected:
		int id;
		std::vector<SensorNode*> nodes;
	public:
		Field();
		Field(int id, int numberOfNodes);
		Field(const Field& field);
		Field& operator=(const Field& field);
		~Field();
		
		SensorNode* operator[](int index);
		int getNumberOfNodes() const;
		int getId() const;
		void addNode(SensorNode* node);
		void deployApplication(OdinExecutor& codeExecutor);
		void configureSink(SensorNode* sink);
		std::vector<SensorNode*>* getAllNodes();
		
	protected:
		template<class Archive>
		void serialize(Archive& archive, const unsigned int version) {
			archive & BOOST_SERIALIZATION_NVP(id);
			archive & boost::serialization::make_nvp("SensorNodes", this->nodes);
		}
	};
}
BOOST_CLASS_VERSION(tinynode::Field, 0)

#endif

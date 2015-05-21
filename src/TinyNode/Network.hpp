/**
 * @file Network.hpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since TinyNode 1.0
 * @date 16/07/2012
 */

#ifndef GRUBI_TINYNODE_NETWORK_HPP_INCLUDED
#define GRUBI_TINYNODE_NETWORK_HPP_INCLUDED

#include <vector>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/version.hpp>

#include "Field.hpp"
#include "OdinExecutor.hpp"

namespace tinynode {
	class Network {
		friend class boost::serialization::access;
	protected:
		std::vector<Field*> fields;
	public:
		Network();
		Network(int numberOfFields);
		Network(const Network& network);
		Network& operator=(const Network& network);
		~Network();
		
		Field* operator[](int index);
		void addField(Field* field);
		void deployApplication(OdinExecutor& codeExecutor);
		void configureSink(SensorNode* sink);
		std::vector<SensorNode*>* getAllNodes();
		
	protected:
		template<class Archive>
		void serialize(Archive& archive, const unsigned int version) {
			archive & boost::serialization::make_nvp("Fields", this->fields);
		}
	};
}
BOOST_CLASS_VERSION(tinynode::Network, 0)

#endif

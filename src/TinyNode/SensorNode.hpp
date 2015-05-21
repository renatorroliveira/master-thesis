/**
 * @file SensorNode.hpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since TinyNode 1.0
 * @date 13/07/2012
 */

#ifndef GRUBI_TINYNODE_SENSORNODE_HPP_INCLUDED
#define GRUBI_TINYNODE_SENSORNODE_HPP_INCLUDED

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>

#include <Odin/Event.hpp>
#include <Odin/Memory.hpp>

#include "Position.hpp"
#include "OdinExecutor.hpp"

namespace tinynode {
	class Simulator;
	class OdinConnector;
	class Field;
	class SensorNode {
		friend class tinynode::Simulator;
		friend class tinynode::OdinConnector;
		friend class tinynode::Field;
		friend class boost::serialization::access;
	protected:
		int id;
		Position position;
		double transmissionRange;
		int activeMemorySize;
		int passiveMemorySize;
		std::vector<odin::Event*> activeMemory;
		std::vector<odin::Event*> passiveMemory;
		bool powerOn;
		odin::Memory* odinActiveMemory;
		odin::Memory* odinPassiveMemory;
		
		OdinExecutor *codeExecutor;
	public:
		SensorNode();
		SensorNode(int id, int activeMemorySize, int passiveMemorySize, double transmissionRange);
		SensorNode(const SensorNode& node);
		SensorNode& operator=(const SensorNode& node);
		~SensorNode();
		
		bool boot(OdinExecutor& codeExecutor);
		bool executeApplication(odin::Connector * connector);
		bool reboot(OdinExecutor& codeExecutor);
		bool shutdown();
		
		Position getPosition() const;
		void setPosition(Position position);
		int getID() const;
		void setID(int id);
		double getTransmissionRange() const;
		void setTransmissionRange(double transmissionRange);
		int getActiveMemorySize() const;
		int getPassiveMemorySize() const;
		bool getActiveMemory(int index) const;
		bool getPassiveMemory(int index) const;
		/// Sets a value into the active memory size and actives the event to execute code.
		void setActiveMemory(int index, bool value);
		/// Sets a value into the passive memory size and actives the event to execute code.
		void setPassiveMemory(int index, bool value);
		/// Can only be set if the node is power off.
		bool setActiveMemorySize(int size);
		/// Can only be set if the node is power off.
		bool setPassiveMemorySize(int size);
		
	protected:
		template<class Archive>
		void serialize(Archive& archive, const unsigned int version) {
			archive & BOOST_SERIALIZATION_NVP(id);
			archive & BOOST_SERIALIZATION_NVP(transmissionRange);
			archive & BOOST_SERIALIZATION_NVP(activeMemorySize);
			archive & BOOST_SERIALIZATION_NVP(passiveMemorySize);
			archive & BOOST_SERIALIZATION_NVP(position);
		}
	};
}

#endif

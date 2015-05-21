/**
 * @file SensorNode.cpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since TinyNode 1.0
 * @date 13/07/2012
 */


#include <GMPI/Logger.hpp>
#include "SensorNode.hpp"
#include <boost/config/no_tr1/complex.hpp>

using namespace loggerpp;

namespace tinynode {
	SensorNode::SensorNode() : position() {
		this->powerOn = false;
		this->id = 0;
		this->activeMemorySize = 10;
		this->passiveMemorySize = 10;
		this->transmissionRange = 15.0f;
		this->activeMemory.reserve(this->activeMemorySize);
		this->passiveMemory.reserve(this->passiveMemorySize);
	}
	SensorNode::SensorNode(int id, int activeMemorySize, int passiveMemorySize, double transmissionRange) : position() {
		this->powerOn = false;
		this->id = id;
		this->activeMemorySize = activeMemorySize;
		this->passiveMemorySize = passiveMemorySize;
		this->transmissionRange = transmissionRange;
		this->activeMemory.reserve(this->activeMemorySize);
		this->passiveMemory.reserve(this->passiveMemorySize);
	}
	SensorNode::SensorNode(const SensorNode& node) {
		*this = node;
	}
	SensorNode& SensorNode::operator=(const SensorNode& node) {
		this->position = node.position;
		this->powerOn = node.powerOn;
		this->id = node.id;
		this->activeMemorySize = node.activeMemorySize;
		this->passiveMemorySize = node.passiveMemorySize;
		this->transmissionRange = node.transmissionRange;
		for (int pos = 0; pos < this->activeMemory.size(); pos++)
			delete [] this->activeMemory[pos];
		this->activeMemory.clear();
		if (this->powerOn && this->activeMemorySize > 0) {
			this->activeMemory.reserve(this->activeMemorySize);
			for (int m = 0; m < this->activeMemorySize; m++) {
				this->activeMemory.push_back(new odin::Event(node.activeMemory[m]));
			}
			if (this->odinActiveMemory != NULL)
				delete this->odinActiveMemory;
			this->odinActiveMemory = new odin::Memory(&(this->activeMemory));
		}
		for (int pos = 0; pos < this->passiveMemory.size(); pos++)
			delete [] this->passiveMemory[pos];
		this->passiveMemory.clear();
		if (this->powerOn && this->passiveMemorySize > 0) {
			this->passiveMemory.reserve(this->passiveMemorySize);
			for (int m = 0; m < this->passiveMemorySize; m++) {
				this->passiveMemory.push_back(new odin::Event(node.passiveMemory[m]));
			}
			if (this->odinPassiveMemory != NULL)
				delete this->odinPassiveMemory;
			this->odinPassiveMemory = new odin::Memory(&(this->passiveMemory));
		}
		return *this;
	}
	SensorNode::~SensorNode() {
		if (this->powerOn) {
			this->shutdown();
		}
	}
		
	bool SensorNode::boot(OdinExecutor& codeExecutor) {
		if (this->powerOn) {
			log("Trying to boot a power on sensor node.", level::WARNING, this);
			return false;
		}
		if (this->activeMemorySize <= 0 || this->passiveMemorySize <= 0) {
			log("Trying to boot a sensor node without memory.", level::SEVERE, this);
			return false;
		}
		for (int m = 0; m < this->activeMemorySize; m++) {
			this->activeMemory.push_back(new odin::Event());
		}
		this->odinActiveMemory = new odin::Memory(&(this->activeMemory));
		for (int m = 0; m < this->passiveMemorySize; m++) {
			this->passiveMemory.push_back(new odin::Event());
		}
		this->odinPassiveMemory = new odin::Memory(&(this->passiveMemory));
		this->powerOn = true;
		this->codeExecutor = &codeExecutor;
		return true;
	}
	bool SensorNode::executeApplication(odin::Connector * connector) {
		if (!this->powerOn) {
			log("Trying to run a power off sensor node.", level::SEVERE, this);
			return false;
		}
		if (this->odinActiveMemory->getMemoryChanged() || this->odinPassiveMemory->getMemoryChanged()) {
			this->odinActiveMemory->setMemoryChanged(false);
			this->odinPassiveMemory->setMemoryChanged(false);
			this->codeExecutor->execute(this, *(this->odinActiveMemory), *(this->odinPassiveMemory));
		}
		return true;
	}
	bool SensorNode::reboot(OdinExecutor& codeExecutor) {
		bool status = true;
		if (this->powerOn)
			status = this->shutdown();
		return (this->boot(codeExecutor) && status);
	}
	bool SensorNode::shutdown() {
		if (!powerOn)
			return false;
		for (int m = 0; m < this->activeMemory.size(); m++) {
			delete this->activeMemory[m];
		}
		delete this->odinActiveMemory;
		this->odinActiveMemory = NULL;
		this->activeMemory.clear();
		for (int m = 0; m < this->passiveMemory.size(); m++) {
			delete this->passiveMemory[m];
		}
		delete this->odinPassiveMemory;
		this->odinPassiveMemory = NULL;
		this->passiveMemory.clear();
		this->codeExecutor = 0;
		this->powerOn = false;
		return true;
	}
		
	Position SensorNode::getPosition() const { return this->position; }
	void SensorNode::setPosition(Position position) { this->position = position; }
	int SensorNode::getID() const { return this->id; }
	void SensorNode::setID(int id) { this->id = id; }
	double SensorNode::getTransmissionRange() const { return this->transmissionRange; }
	void SensorNode::setTransmissionRange(double transmissionRange) { this->transmissionRange = transmissionRange; }
	int SensorNode::getActiveMemorySize() const { return this->activeMemorySize; }
	int SensorNode::getPassiveMemorySize() const { return this->passiveMemorySize; }
	bool SensorNode::getActiveMemory(int index) const { return this->activeMemory[index]->getFired(); }
	bool SensorNode::getPassiveMemory(int index) const { return this->passiveMemory[index]->getFired(); }
	
	void SensorNode::setActiveMemory(int index, bool value) {
		if (this->powerOn) {
			this->odinActiveMemory->setMemoryAt(index, value);
		} else {
			log("Error trying to set memory on a node.", level::FATAL_ERROR, this);
		}
	}
	void SensorNode::setPassiveMemory(int index, bool value) {
		if (this->powerOn) {
			this->odinPassiveMemory->setMemoryAt(index, value);
		} else {
			log("Error trying to set memory on a node.", level::FATAL_ERROR, this);
		}
	}
	
	bool SensorNode::setActiveMemorySize(int size) {
		if (powerOn)
			return false;
		this->activeMemorySize = size;
		return true;
	}
	bool SensorNode::setPassiveMemorySize(int size) {
		if (powerOn)
			return false;
		this->passiveMemorySize = size;
		return true;
	}
}

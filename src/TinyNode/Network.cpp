/**
 * @file Network.cpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since TinyNode 1.0
 * @date 16/07/2012
 */

#include "Network.hpp"

namespace tinynode {
	Network::Network() { }
	Network::Network(int numberOfFields) {
		this->fields.reserve(numberOfFields);
	}
	Network::Network(const Network& network) {
		*this = network;
	}
	Network& Network::operator=(const Network& network) {
		for (int field = 0; field < this->fields.size(); field++) {
			delete this->fields[field];
		}
		this->fields.clear();
		if (network.fields.size() > 0) {
			this->fields.reserve(network.fields.size());
			for (int field = 0; field < network.fields.size(); field++) {
				this->fields.push_back(new Field(*(network.fields[field])));
			}
		}
	}
	Network::~Network() {
		for (int field = 0; field < this->fields.size(); field++) {
			delete this->fields[field];
		}
		this->fields.clear();
	}
	
	Field* Network::operator[](int index) {
		if (index < 0 || index >= this->fields.size())
			return 0;
		return this->fields[index];
	}
	
	void Network::addField(Field* field) {
		if (this->fields.capacity() == this->fields.size())
			this->fields.reserve(this->fields.size()+1);
		this->fields.push_back(field);
	}
	
	void Network::deployApplication(OdinExecutor& codeExecutor) {
		for (int field = 0; field < this->fields.size(); field++) {
			this->fields[field]->deployApplication(codeExecutor);
		}
	}
	
	void Network::configureSink(SensorNode* sink) {
		for (int field = 0; field < this->fields.size(); field++) {
			this->fields[field]->configureSink(sink);
		}
	}
	
	std::vector<SensorNode*>* Network::getAllNodes() {
		std::vector<SensorNode*>* nodes = new std::vector<SensorNode*>();
		for (int f = 0; f < this->fields.size(); f++) {
			std::vector<SensorNode*>* nodesInField = this->fields[f]->getAllNodes();
			for (int n = 0; n < nodesInField->size(); n++) {
				nodes->push_back(nodesInField->at(n));
			}
			nodesInField->clear();
			delete nodesInField;
		}
		return nodes;
	}
}

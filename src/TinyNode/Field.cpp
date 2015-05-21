/**
 * @file Field.cpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since TinyNode 1.0
 * @date 13/07/2012
 */

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "Field.hpp"

namespace tinynode {
	Field::Field() {
		this->id = 0;
	}
	
	Field::Field(int id, int numberOfNodes) {
		this->id = id;
		this->nodes.reserve(numberOfNodes);
	}
	
	Field::Field(const Field& field) {
		*this = field;
	}
	Field& Field::operator=(const Field& field) {
		this->id = field.id;
		for (int node = 0; node < this->nodes.size(); node++) {
			delete this->nodes[node];
		}
		this->nodes.clear();
		if (field.nodes.size() > 0) {
			this->nodes.reserve(field.nodes.size());
			for (int node = 0; node < field.nodes.size(); node++) {
				this->nodes.push_back(new SensorNode(*(field.nodes[node])));
			}
		}
	}
	
	Field::~Field() {
		for (int node = 0; node < this->nodes.size(); node++) {
			delete this->nodes[node];
		}
	}
	
	SensorNode* Field::operator[](int index) {
		if (index < 0 || index >= this->nodes.size())
			return 0;
		return this->nodes[index];
	}
	
	void Field::addNode(SensorNode* node) {
		if (this->nodes.capacity() == this->nodes.size())
			this->nodes.reserve(this->nodes.size()+1);
		this->nodes.push_back(node);
	}
	
	void Field::deployApplication(OdinExecutor& codeExecutor) {
		for (int node = 0; node < this->nodes.size(); node++) {
			this->nodes[node]->reboot(codeExecutor);
		}
	}
	
	void Field::configureSink(SensorNode* sink) {
		if (sink == NULL)
			loggerpp::log("Trying to configure a NULL sink node.\n", loggerpp::level::FATAL_ERROR, this);

		std::vector<SensorNode*>::iterator it;
		for (it = this->nodes.begin(); it != this->nodes.end(); it++) 
		{
			SensorNode* node = (*it);

			if (node->activeMemorySize < 5)
				loggerpp::log("Insuficient active memory size, minimum is 5.\n", loggerpp::level::FATAL_ERROR, this);

			int base = node->activeMemorySize - 4;

			
			if (node->getID() != sink->getID()) /* If node is not the sink node */
			{
				Position sinkPos = sink->getPosition();		/* Sink node position. */
				Position nodePos = node->getPosition();		/* Node position. */

				double xDist = 0.0f, yDist = 0.0f;

				if (sinkPos.getX() >= nodePos.getX()) 		/* Se sinkNode está a direita de node */
				{
					xDist = sinkPos.getX() - nodePos.getX();	

					if (sinkPos.getY() >= nodePos.getY()) 	/* Se sinkNode está acima de node. */
					{
						yDist = sinkPos.getY() - nodePos.getY();

						if (xDist >= yDist) {
							node->activeMemory.at(base+1)->setFired(true);
						} else {
							node->activeMemory.at(base)->setFired(true);
						}
					} else {
						yDist = nodePos.getY() - sinkPos.getY();
						if (xDist >= yDist) {
							node->activeMemory.at(base+1)->setFired(true);
						} else {
							node->activeMemory.at(base+2)->setFired(true);
						}
					}
				} else {
					xDist = nodePos.getX() - sinkPos.getX();
					if (sinkPos.getY() >= nodePos.getY()) {
						yDist = sinkPos.getY() - nodePos.getY();
						if (xDist >= yDist) {
							node->activeMemory.at(base+3)->setFired(true);
						} else {
							node->activeMemory.at(base)->setFired(true);
						}
					} else {
						yDist = nodePos.getY() - sinkPos.getY();
						if (xDist >= yDist) {
							node->activeMemory.at(base+3)->setFired(true);
						} else {
							node->activeMemory.at(base+2)->setFired(true);
						}
					}
				}
			}
			/*std::cout << "Node " << std::setw(3) << node->getID() << ":  ";
			std::cout << " " << (node->getActiveMemory(base) ? "1":"0") <<
				" " << (node->getActiveMemory(base+1) ? "1":"0") <<
				" " << (node->getActiveMemory(base+2) ? "1":"0") <<
				" " << (node->getActiveMemory(base+3) ? "1":"0") << std::endl;*/
		}
		//std::exit(0);
	}
	
	std::vector<SensorNode*>* Field::getAllNodes() {
		std::vector<SensorNode*>* nds = new std::vector<SensorNode*>();
		for (int n = 0; n < this->nodes.size(); n++) {
			nds->push_back(this->nodes[n]);
		}
		return nds;
	}
	
	int Field::getNumberOfNodes() const { return this->nodes.size(); }
	int Field::getId() const { return this->id; }
}

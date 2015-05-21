#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <boost/archive/xml_archive_exception.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <TinyNode/EventTrigger.hpp>
#include <TinyNode/Network.hpp>
#include <TinyNode/Simulator.hpp>

using namespace std;
using namespace boost;
using namespace tinynode;

class InstanceSetup {
public:
	InstanceSetup() {}
	
	string instanceBaseName;
	int numberOfInstances;
	double fieldSizeX, fieldSizeY;
	int numberOfNodes;
	int activeMemorySize;
	int passiveMemorySize;
	double transmissionRange;
	int nodePlacementType;
	int numberOfSinkNodes;
	int numberOfEventTriggers;
	int numberOfEventsToTrigger;
	double simulationMaxTime;
	double nodesSleepCycle;
	
	template<class Archive>
	void serialize(Archive& archive, const unsigned int version) {
		archive & BOOST_SERIALIZATION_NVP(instanceBaseName);
		archive & BOOST_SERIALIZATION_NVP(numberOfInstances);
		archive & BOOST_SERIALIZATION_NVP(fieldSizeX);
		archive & BOOST_SERIALIZATION_NVP(fieldSizeY);
		archive & BOOST_SERIALIZATION_NVP(numberOfNodes);
		archive & BOOST_SERIALIZATION_NVP(activeMemorySize);
		archive & BOOST_SERIALIZATION_NVP(passiveMemorySize);
		archive & BOOST_SERIALIZATION_NVP(transmissionRange);
		archive & BOOST_SERIALIZATION_NVP(nodePlacementType);
		archive & BOOST_SERIALIZATION_NVP(numberOfSinkNodes);
		archive & BOOST_SERIALIZATION_NVP(numberOfEventTriggers);
		archive & BOOST_SERIALIZATION_NVP(numberOfEventsToTrigger);
		archive & BOOST_SERIALIZATION_NVP(simulationMaxTime);
		archive & BOOST_SERIALIZATION_NVP(nodesSleepCycle);
	}
};


// Prototypes:
int selectSink(Field*, int, InstanceSetup*);
int selectTriggeredNode(Field*, int, InstanceSetup*);
bool isConnectedPosition(Position&, vector<SensorNode*>*, InstanceSetup*);


int main(int argc, char** argv) {
	try {
	int numberOfSets;
	InstanceSetup *setups;
	{
		string confFile = "generator.xml";
		if (argc > 1)
			confFile = argv[1];
		cout << "Reading configuration: " << confFile << endl;
		ifstream input(confFile.c_str(), ios::in);
		archive::xml_iarchive inarch(input);
		inarch >> BOOST_SERIALIZATION_NVP(numberOfSets);
		if (numberOfSets <= 0)
			return -1;
		setups = new InstanceSetup[numberOfSets];
		for (int set = 0; set < numberOfSets; set++) {
			inarch >> serialization::make_nvp("InstanceSetup", setups[set]);
		}
	}
	
	// Generating instances.
	random::mt19937 rng;
	random::uniform_int_distribution<> intrand;
	random::uniform_real_distribution<> realrand;
	rng.seed(time(0));
	for (int set = 0; set < numberOfSets; set++) {
		InstanceSetup *setup = &(setups[set]);
		for (int inst = 0; inst < setup->numberOfInstances; inst++) {
			string name;
			if (setup->numberOfInstances == 1) {
				name = setup->instanceBaseName+".xml";
			} else {
				stringstream sout;
				sout << setup->instanceBaseName << "_" << inst << ".xml";
				name = sout.str();
			}
			fstream out(name.c_str(), ios::out);
			archive::xml_oarchive output(out);
			Network network(1);
			Field *field = new Field(0, setup->numberOfNodes);
			for (int node = 0; node < setup->numberOfNodes; node++) {
				SensorNode *newNode = new SensorNode(node, setup->activeMemorySize, setup->passiveMemorySize, setup->transmissionRange);
				int posX, posY;
				double square;
				switch (setup->nodePlacementType) {
					case 0:
						square = sqrt(setup->numberOfNodes);
						posX = node%(int)square;
						posY = node/square;
						newNode->setPosition(Position(posX*setup->fieldSizeX/(square-1.0f), posY*setup->fieldSizeY/(square-1.0f), 0.0f));
						break;
					case 1:
						{
							Position pos(setup->fieldSizeX*realrand(rng), setup->fieldSizeY*realrand(rng), 0.0f);
							// Asserting a connected network.
							while (!isConnectedPosition(pos, field->getAllNodes(), setup)) {
								pos = Position(setup->fieldSizeX*realrand(rng), setup->fieldSizeY*realrand(rng), 0.0f);
							}
							newNode->setPosition(pos);
						}
						break;
					case 2:
						{
								Position pos(setup->fieldSizeX*realrand(rng), setup->fieldSizeY*realrand(rng), 0.0f);
								// Just insert a random positioned node.
								newNode->setPosition(pos);
						}
						break;
					default:
						cerr << "Invalid placement type: " << setup->nodePlacementType << endl;
						abort();
				}
				field->addNode(newNode);
			}
			network.addField(field);
			simulation::Setup simulationSetup;
			simulationSetup.nodesActiveMemorySize = setup->activeMemorySize;
			simulationSetup.nodesPassiveMemorySize = setup->passiveMemorySize;
			simulationSetup.nodesSleepCycle = setup->nodesSleepCycle;
			simulationSetup.numberOfNodes = setup->numberOfNodes;
			simulationSetup.simulationMaxTime = setup->simulationMaxTime;
			std::vector<int> nodesIDS(setup->numberOfNodes);
			for (int node = 0; node < setup->numberOfNodes; node++)
				nodesIDS[node] = node;
			for (int sink = 0; sink < setup->numberOfSinkNodes; sink++) {
				simulationSetup.sinkNodes.push_back(selectSink(field, sink, setup));
			}
			for (int event = 0; event < setup->numberOfEventsToTrigger; event++) {
				for (int t = 0; t < setup->numberOfEventTriggers; t++) {
					int nodeId = selectTriggeredNode(field, ((event * setup->numberOfEventTriggers)+t), setup);
					double ocurrenceTime = realrand(rng)*setup->simulationMaxTime/2.0f;
					simulationSetup.eventTriggers.push_back(new EventTrigger(event, nodeId, ocurrenceTime));
				}
			}
			output << serialization::make_nvp("SimulationSetup", simulationSetup);
			output << serialization::make_nvp("Network", network);
		}
	}
	} catch (archive::xml_archive_exception ex) {
		cerr << "ERROR! XML Archive:" << endl << ex.what() << endl;
	}
	
	return 0;
}


int selectSink(Field* field, int sink, InstanceSetup *setup) {
	vector<SensorNode*>* nodes = field->getAllNodes();
	int sinkId = -1;
	if (sink == 0) {
		SensorNode* centralNode = 0;
		double bestDistance = sqrt(pow(setup->fieldSizeX,2.0f)+pow(setup->fieldSizeY,2.0f));
		Position centralPosistion(setup->fieldSizeX/2.0f, setup->fieldSizeY/2.0f, 0.0f);
		for (size_t n = 0; n < nodes->size(); n++) {
			double dist = centralPosistion.distance(nodes->at(n)->getPosition());
			if (dist < bestDistance) {
				centralNode = nodes->at(n);
				bestDistance = dist;
			}
		}
		sinkId = centralNode->getID();
	} else {
		sinkId = nodes->at(((int)pow(sink,2))%(nodes->size()))->getID();
	}
	return sinkId;
}

int selectTriggeredNode(Field* field, int event, InstanceSetup* setup) {
	vector<SensorNode*>* nodes = field->getAllNodes();
	int nodeId = -1;
	SensorNode* choosenNode = 0;
	double bestDistance = sqrt(pow(setup->fieldSizeX,2.0f)+pow(setup->fieldSizeY,2.0f));
	Position *desiredPosistion;
	switch (event) {
		case 0:
			desiredPosistion = new Position(setup->fieldSizeX/4.0f, setup->fieldSizeY/4.0f, 0.0f);
			break;
		case 1:
			desiredPosistion = new Position(3.0f*setup->fieldSizeX/4.0f, 3.0f*setup->fieldSizeY/4.0f, 0.0f);
			break;
		case 2:
			desiredPosistion = new Position(3.0f*setup->fieldSizeX/4.0f, setup->fieldSizeY/4.0f, 0.0f);
			break;
		case 3:
			desiredPosistion = new Position(setup->fieldSizeX/4.0f, 3.0f*setup->fieldSizeY/4.0f, 0.0f);
			break;
		case 4:
			desiredPosistion = new Position(setup->fieldSizeX/8.0f, setup->fieldSizeY/8.0f, 0.0f);
			break;
		case 5:
			desiredPosistion = new Position(7.0f*setup->fieldSizeX/8.0f, 7.0f*setup->fieldSizeY/8.0f, 0.0f);
			break;
		case 6:
			desiredPosistion = new Position(7.0f*setup->fieldSizeX/8.0f, setup->fieldSizeY/8.0f, 0.0f);
			break;
		case 7:
			desiredPosistion = new Position(setup->fieldSizeX/8.0f, 7.0f*setup->fieldSizeY/8.0f, 0.0f);
			break;
		case 8:
			desiredPosistion = new Position(setup->fieldSizeX/12.0f, setup->fieldSizeY/12.0f, 0.0f);
			break;
		case 9:
			desiredPosistion = new Position(11.0f*setup->fieldSizeX/12.0f, 11.0f*setup->fieldSizeY/12.0f, 0.0f);
			break;
		case 10:
			desiredPosistion = new Position(11.0f*setup->fieldSizeX/12.0f, setup->fieldSizeY/12.0f, 0.0f);
			break;
		case 11:
			desiredPosistion = new Position(setup->fieldSizeX/12.0f, 11.0f*setup->fieldSizeY/12.0f, 0.0f);
			break;
		default:
			cerr << "Invalid number of events: " << event << endl;
			abort();
	}
	for (size_t n = 0; n < nodes->size(); n++) {
		double dist = desiredPosistion->distance(nodes->at(n)->getPosition());
		if (dist < bestDistance) {
			choosenNode = nodes->at(n);
			bestDistance = dist;
		}
	}
	nodeId = choosenNode->getID();
	return nodeId;
}

bool isConnectedPosition(Position& pos, vector<SensorNode*>* nodes, InstanceSetup* setup) {
	if (nodes->size() <= 0)
		return true;
	for (int n = 0; n < nodes->size(); n++) {
		if (((pos % nodes->at(n)->getPosition()) - setup->transmissionRange) <= -0.1f)
			return true;
	}
	return false;
}



#include "EventTrigger.hpp"

namespace tinynode {
	EventTrigger::EventTrigger() {
		this->event_id = 0;
		this->node_id = 0;
		this->ocurrence_time = 0.0f;
		this->memory = NULL;
		this->triggered = false;
	}
	EventTrigger::EventTrigger(int event, int node, double ocurrence) {
		this->event_id = event;
		this->node_id = node;
		this->ocurrence_time = ocurrence;
		this->memory = NULL;
		this->triggered = false;
	}
	EventTrigger::~EventTrigger() { }
		
	bool EventTrigger::trigger(double currentTime) {
		if (this->memory == 0)
			return false;
		if (this->triggered) {
			return true;
		} else if ((*this) <= currentTime) {
			this->memory->setMemoryAt(this->event_id, true);
			this->triggered = true;
			return true;
		} else {
			return false;
		}
	}
	double EventTrigger::getOcurrenceTime() const {
		return this->ocurrence_time;
	}
	int EventTrigger::getNodeID() const {
		return this->node_id;
	}
	int EventTrigger::getEventID() const {
		return this->event_id;
	}
	odin::Event* EventTrigger::getEvent() const {
		return this->memory->getMemoryAt(this->event_id);
	}
	void EventTrigger::setMemory(odin::Memory* memory) {
		this->memory = memory;
		this->triggered = false;
	}
	
	bool EventTrigger::less_than(EventTrigger * trigger1, EventTrigger * trigger2) {
		return ((*trigger1) < (*trigger2));
	}
	bool EventTrigger::operator<(const EventTrigger& trigger) const {
		if (this->event_id == trigger.event_id) {
			return (this->ocurrence_time < trigger.ocurrence_time);
		}
		return (this->event_id < trigger.event_id);
	}
	bool EventTrigger::operator<=(double currentTime) const {
		return (this->ocurrence_time <= currentTime);
	}
}

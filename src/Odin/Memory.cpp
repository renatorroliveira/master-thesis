/**
 * \author Renato R. R. de Oliveira (renatorro@comp.ufla.br)
 */

#include <cstdlib>
#include "Memory.hpp"


namespace odin {
	Memory::Memory(std::vector<Event*>* memory) {
		if (memory == NULL)
			std::abort();
		this->memory = memory;
		this->memoryChanged = true;
	}

	Memory::Memory(const Memory& memory) {
		*this = memory;
	}
	Memory& Memory::operator=(const Memory& memory) {
		this->memory = memory.memory;
		this->memoryChanged = true;
	}

	Memory::~Memory() {}

	bool Memory::getMemoryChanged() const {
		return this->memoryChanged;
	}
	Event* Memory::getMemoryAt(const int index) const {
		if ((this->memory->size() > index) && (index >= 0))
			return this->memory->at(index);
		else
			return NULL;
	}
	Event* Memory::operator[](const int index) const {
		return this->getMemoryAt(index);
	}

	void Memory::setMemoryChanged(bool value) {
		this->memoryChanged = value;
	}
	void Memory::setMemoryAt(int index, bool value) {
		if ((this->memory->size() > index) && (index >= 0)) {
			if (this->memory->at(index)->getFired() != value) {
				this->memory->at(index)->setFired(value);
				this->memoryChanged = true;
			}
		} else
			std::abort();
	}
}

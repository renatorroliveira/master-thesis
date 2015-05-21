/**
 * \file Memory.hpp
 * \author Renato R. R. de Oliveira (renatorro@comp.ufla.br)
 * \version 1.0
 * \since Odin 1.0
 * \date 22/08/2012
 */

#ifndef GRUBI_ODIN_MEMORY_HPP_INCLUDED
#define GRUBI_ODIN_MEMORY_HPP_INCLUDED

#include <vector>

#include "Event.hpp"

namespace odin {
	class Memory {
	protected:
		std::vector<Event*>* memory;
		bool memoryChanged;
	public:
		Memory(std::vector<Event*>* memory);
		Memory(const Memory& memory);
		Memory& operator=(const Memory& memory);
		~Memory();

		bool getMemoryChanged() const;
		Event* getMemoryAt(const int index) const;
		Event* operator[](const int index) const;
		void setMemoryChanged(bool value);
		void setMemoryAt(int index, bool value);
	};
}

#endif

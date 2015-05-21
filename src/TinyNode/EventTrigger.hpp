/**
 * @file EventTrigger.hpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since TinyNode 1.0
 * @date 18/07/2012
 */

#ifndef GRUBI_TINYNODE_EVENTTRIGGER_HPP_INCLUDED
#define GRUBI_TINYNODE_EVENTTRIGGER_HPP_INCLUDED

#include <vector>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include <Odin/Event.hpp>
#include <Odin/Memory.hpp>

namespace tinynode {
	class EventTrigger {
		friend class boost::serialization::access;
	protected:
		int event_id;
		int node_id;
		double ocurrence_time;
		odin::Memory* memory;
		bool triggered;
	public:
		EventTrigger();
		EventTrigger(int event, int node, double ocurrence);
		~EventTrigger();
		
		/// Checks the time and triggers it if necessary. Returns if triggered or not.
		bool trigger(double currentTime);
		int getEventID() const;
		int getNodeID() const;
		double getOcurrenceTime() const;
		odin::Event* getEvent() const;
		void setMemory(odin::Memory* memory);
		
		/** Function that compares two event triggers. To be used with std::sort()
		 *  when working with pointers to objects. */
		static bool less_than(EventTrigger * trigger1, EventTrigger * trigger2);
		/** This operator is useful when using std::sort() with pure objects. */
		bool operator<(const EventTrigger& trigger) const;
		/// Useful to check if this event ocurrence time is reached.
		bool operator<=(double currentTime) const;
		
	protected:
		template<class Archive>
		void serialize(Archive& archive, const unsigned int version) {
			archive & BOOST_SERIALIZATION_NVP(event_id);
			archive & BOOST_SERIALIZATION_NVP(node_id);
			archive & BOOST_SERIALIZATION_NVP(ocurrence_time);
		}
	};
}

#endif

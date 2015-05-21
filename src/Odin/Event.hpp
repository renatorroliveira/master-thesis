 /**
  * @file Event.hpp
 * @author Renato R. R. de Oliveira (renatorro@comp.ufla.br)
  * @version 1.0
  * @since Odin 1.0
  * @date 05/07/2012
  */

#ifndef GRUBI_ODIN_EVENT_HPP
#define GRUBI_ODIN_EVENT_HPP

// Estes includes são necessários para a serialização.
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>

namespace odin {

	/**
	 * Interface da classe de eventos do middleware. Atualmente, um evento é apenas um booleano.
	 * Não existe nenhuma necessidade de criar como uma interface.
	 */
	class Event {
		// Permite que a classe seja serializada.
		friend class boost::serialization::access;
	public:
		Event() {
			this->fired = false;
		}
		Event(bool fired) {
			this->fired = fired;
		}
		Event(const Event& event) {
			*this = event;
		}
		~Event() {}
		void setFired(bool fired) {
			this->fired = fired;
		}
		bool getFired() {
			return this->fired;
		}

		Event& operator=(const Event& event) {
			this->fired = event.fired;
			return *this;
		}

	protected:
		bool fired;

		/// Serialização da classe.
		template<class Archive>
		void serialize(Archive & archive, const unsigned int version) {
			/*
			 * NVP significa Name-Value Pair. Isso permite que a serialização
			 * possa ser realizada tambem em formato XML. Pode-se usar também
			 * o boost::make_nvp diretamente. Essa macro utiliza o nome do campo
			 * para ser o nome da tag XML.
			 */
			archive & BOOST_SERIALIZATION_NVP(fired);
        }
	};
}
#endif

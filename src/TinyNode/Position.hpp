/**
 * @file Position.hpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since TinyNode 1.0
 * @date 13/07/2012
 */

#ifndef GRUBI_TINYNODE_POSITION_HPP_INCLUDED
#define GRUBI_TINYNODE_POSITION_HPP_INCLUDED

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>

namespace tinynode {
	/** @brief This class represents a position in the real world (3D). */
	class Position {
		friend class boost::serialization::access;
	protected:
		double x, y, z;
	public:
		Position();
		Position(double x, double y, double z);
		Position(const Position& position);
		~Position() { /* Nothing to be done.*/ }
		
		Position& operator=(const Position& position);
		bool operator==(const Position& position);
		/// Calculates the euclidean's distance between two positions.
		double operator%(const Position& position);
		/// Calculates the euclidean's distance between two positions.
		double distance(const Position& position);
		
		double getX() const;
		double getY() const;
		double getZ() const;
		void setX(double value);
		void setY(double value);
		void setZ(double value);
	protected:
		template<class Archive>
		void serialize(Archive& archive, const unsigned int version) {
			archive & BOOST_SERIALIZATION_NVP(x);
			archive & BOOST_SERIALIZATION_NVP(y);
			archive & BOOST_SERIALIZATION_NVP(z);
		}
	};
}

#endif

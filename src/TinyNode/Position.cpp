/**
 * @file Position.cpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since TinyNode 1.0
 * @date 13/07/2012
 */

#include "Position.hpp"
#include <cmath>

namespace tinynode {
	Position::Position() {
		this->x = 0.0f;
		this->y = 0.0f;
		this->z = 0.0f;
	}
	
	Position::Position(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	
	Position::Position(const Position& position) {
		*this = position;
	}
	
	Position& Position::operator=(const Position& position) {
		this->x = position.x;
		this->y = position.y;
		this->z = position.z;
		return *this;
	}
	
	bool Position::operator==(const Position& position) {
		return (this->x == position.x && this->y == position.y && this->z == position.z);
	}
	
	double Position::operator%(const Position& position) {
		double distance = std::pow(this->x-position.x, 2);
		distance += std::pow(this->y-position.y, 2);
		distance += std::pow(this->z-position.z, 2);
		return std::sqrt(distance);
	}
	double Position::distance(const Position& position) {
		return (*this % position);
	}
	
	double Position::getX() const { return this->x; }
	double Position::getY() const { return this->y; }
	double Position::getZ() const { return this->z; }
	void Position::setX(double value) { this->x = value; }
	void Position::setY(double value) { this->y = value; }
	void Position::setZ(double value) { this->z = value; }
}

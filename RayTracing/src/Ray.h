#pragma once

#include"glm\glm.hpp"


struct Ray {
	Ray() {}
	Ray(glm::vec3 Origin, glm::vec3 Direction)
	: m_Origin(Origin), m_Direction(Direction){}
	
	glm::vec3 m_Origin;
	glm::vec3 m_Direction;
	float m_T;


	glm::vec3 at(float t) const {
		return m_Origin + t * m_Direction;
	}
};
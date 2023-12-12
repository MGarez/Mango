#pragma once

#include <glm/glm.hpp>
#include "core.h"

struct RigidBodyComponent
{
	RigidBodyComponent(glm::vec2 velocity = glm::vec2(0.,0.)) :
		m_velocity(velocity){}

	glm::vec2 m_velocity;
};


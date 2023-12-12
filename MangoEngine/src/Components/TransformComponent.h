#pragma once
#include "core.h"
#include <glm/glm.hpp>

struct TransformComponent
{
	TransformComponent(glm::vec2 position = glm::vec2(0.,0.), glm::vec2 scale = glm::vec2(0., 0.), double rotation = 0.) :
		m_position(position), m_scale (scale), m_rotation(rotation)
	{
	}

	glm::vec2 m_position;
	glm::vec2 m_scale;
	double m_rotation;
};


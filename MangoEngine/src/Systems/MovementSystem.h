#pragma once
#include "core.h"

class MovementSystem : public System
{
public:

	MovementSystem();
	void Update(const float & deltaTime);
};
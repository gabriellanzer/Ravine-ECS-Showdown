#pragma once
#include <glm/glm.hpp>

class IAccelerable
{
protected:
	virtual void accelerate(glm::vec2 acc, double deltaTime) = 0;

public:
	glm::vec2 velocity;
	IAccelerable() = delete;
	IAccelerable(glm::vec2 vel) : velocity(vel) { };

};
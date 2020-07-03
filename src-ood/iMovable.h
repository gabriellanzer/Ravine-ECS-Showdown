#pragma once
#include <glm/glm.hpp>

class IMovable
{
protected:
	virtual void move(glm::vec2 vel, double deltaTime) = 0;

public:
	glm::vec2 position;
	IMovable() = delete;
	IMovable(glm::vec2 pos) : position(pos) { };

};
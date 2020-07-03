#pragma once

#include "iAccelerable.h"
#include "iMovable.h"

class RigidBody : public IAccelerable, IMovable
{
private:
	// Inherited via IMovable
	virtual void move(glm::vec2 vel, double deltaTime) override;

protected:
	RigidBody() = delete;
	RigidBody(glm::vec2 pos, glm::vec2 vel);

public:
	IAccelerable::velocity;
	IMovable::position;

	// Inherited via IAccelerable
	virtual void accelerate(glm::vec2 acc, double deltaTime) override;

	virtual void update(double deltaTime);
};
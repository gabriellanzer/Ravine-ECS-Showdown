#include "rigidBody.h"

void RigidBody::move(glm::vec2 vel, double deltaTime)
{
	position += vel * (float)deltaTime;
}

RigidBody::RigidBody(glm::vec2 pos, glm::vec2 vel) : IMovable(pos), IAccelerable(vel)
{
	
}

void RigidBody::accelerate(glm::vec2 acc, double deltaTime)
{
	velocity += acc * (float)deltaTime;
}

void RigidBody::update(double deltaTime)
{
	this->move(velocity, deltaTime);

    float absX = abs(position.x);
    float absY = abs(position.y);
    if (absX > 1.0f)
    {
        velocity.x *= -0.9;
        if (position.x < 0)
        {
            position.x = -1.0;
        }
        else
        {
            position.x = 1.0;
        }
    }

    if (absY > 1.0f)
    {
        velocity.y *= -0.9;
        if (position.y < 0)
        {
            position.y = -1.0;
        }
        else
        {
            position.y = 1.0;
        }
    }
}
#pragma once

#include <glm/glm.hpp>
#include "iDrawable.h"
#include "rigidBody.h"
#include "mouse.h"

class Ball : public RigidBody, IDrawable
{
private:
	float radius;
	Mouse* mouse;
	GLuint posLoc;

public:
	Ball(float radius, glm::vec2 pos, glm::vec2 vel, GLint posLoc, GLuint program, GLuint vao, Mouse* mouse);

	// Inherited via IDrawable
	virtual void draw() override;

	void update(double deltaTime);
};
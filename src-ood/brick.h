#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "ball.h"

using std::vector;

class Brick : IDrawable
{
private:
	glm::vec4 rect;
	GLint brickRectLoc;
	GLint brickHpLoc;

public:
	int hp;

	Brick(glm::vec4 rect, int health, GLint brickRectLoc, GLint brickHpLoc, GLuint program, GLuint vao);

	// Inherited via IDrawable
	virtual void draw() override;

	void update(vector<Ball>& ballList, double dt);
};
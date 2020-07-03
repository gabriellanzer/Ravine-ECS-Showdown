#include "brick.h"

Brick::Brick(glm::vec4 rect, int health, GLint brickRectLoc, GLint brickHpLoc, GLuint program, GLuint vao) :
	rect(rect), hp(health), brickRectLoc(brickRectLoc), brickHpLoc(brickHpLoc), IDrawable(program, vao)
{
	this->rect.z *= 0.5f;
	this->rect.w *= 0.5f;
}

void Brick::draw()
{
	glUseProgram(program);
	glUniform4f(brickRectLoc, rect.x, rect.y, rect.z, rect.w);
	glUniform1i(brickHpLoc, hp);
	glBindVertexArray(vao);
	glDrawArrays(GL_QUADS, 0, 4);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Brick::update(vector<Ball>& ballList, double dt)
{
	for (size_t i = 0; i < ballList.size(); i++)
	{
		glm::vec2& p = ballList[i].position;
		glm::vec2& v = ballList[i].velocity;

		if (abs((p.x + v.x * dt) - rect.x) > (rect.w + 0.01f))
		{
			continue;
		}
		if (abs((p.y + v.y * dt) - rect.y) > (rect.z + 0.01f))
		{
			continue;
		}

		hp--;

		if (p.y < rect.y - (rect.w + 0.01f))
		{
			p.y = rect.y - (rect.w + 0.01f);
			v.y *= -1;
			continue;
		}
		if (p.x < rect.x - (rect.z + 0.01f))
		{
			p.x = rect.x - (rect.z + 0.01f);
			v.x *= -1;
			continue;
		}
		if (p.x > rect.x + (rect.z + 0.01f))
		{
			p.x = rect.x + (rect.z + 0.01f);
			v.x *= -1;
			continue;
		}
		if (p.y > rect.y + (rect.w + 0.01f))
		{
			p.y = rect.y + (rect.w + 0.01f);
			v.y *= -1;
			continue;
		}
	}
}

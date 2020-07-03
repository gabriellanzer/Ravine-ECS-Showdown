#ifndef BOXSYSTEM_H
#define BOXSYSTEM_H

#include <vector>

#include <glad/glad.h>
#include <glm/geometric.hpp>

#include <ravine/ecs/BaseSystem.hpp>

#include "shader.hpp"
#include "io.hpp"

#include "rect.h"
#include "position.h"
#include "velocity.h"

using namespace rv;
using std::abs;
using std::vector;

static const struct
{
	float x, y;
} brickMesh[4]{
	{ -1.f, -1.f },
	{ +1.f, -1.f },
	{ +1.f, +1.f },
	{ -1.f, +1.f }
};


class BoxSystem : public BaseSystem<Position, Velocity>
{
private:
	vector<Rect> rects;
	vector<int> hps;
	GLint brickShaderProg;
	GLint countLoc;
	GLint brickRectLoc;
	GLint brickHpLoc;
	GLuint brickVAO;
	GLuint brickVertShader;
	GLuint brickFragShader;
	GLuint brickMeshBuffer;

public:
	BoxSystem()
	{
		rects.reserve(200);
		hps.reserve(200);

		// Load Vertex Shader
		char* shaderData;
		rvLoadFile("../shaders/vert_brick_instanced.shader", shaderData, true);
		brickVertShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(brickVertShader, 1, &shaderData, NULL);
		glCompileShader(brickVertShader);
		if (!checkShaderCompile(brickVertShader))
		{
			getchar();
			return;
		}
		// Load Fragment Shader
		rvLoadFile("../shaders/frag_col.shader", shaderData, true);
		brickFragShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(brickFragShader, 1, &shaderData, NULL);
		glCompileShader(brickFragShader);
		if (!checkShaderCompile(brickFragShader))
		{
			getchar();
			return;
		}
		// Create Pipeline Program
		brickShaderProg = glCreateProgram();
		glAttachShader(brickShaderProg, brickVertShader);
		glAttachShader(brickShaderProg, brickFragShader);
		glLinkProgram(brickShaderProg);
		if (!checkProgramLink(brickShaderProg))
		{
			getchar();
			return;
		}
		// Get Uniform Locations
		brickRectLoc = glGetUniformLocation(brickShaderProg, "boxRect");
		brickHpLoc = glGetUniformLocation(brickShaderProg, "boxHp");

		// Setup Array Buffer
		glGenVertexArrays(1, &brickVAO);
		glBindVertexArray(brickVAO);
		// Load Quad Buffer
		glGenBuffers(1, &brickMeshBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, brickMeshBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(brickMesh), brickMesh, GL_STATIC_DRAW);
		// Setup Bindings
		GLint vpos_location = glGetAttribLocation(brickShaderProg, "vertPos");
		glEnableVertexAttribArray(vpos_location);
		glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
			sizeof(brickMesh[0]), (void*)0);
		// Unbind Buffers
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	~BoxSystem()
	{
		glDeleteProgram(brickShaderProg);
		glDeleteShader(brickVertShader);
		glDeleteShader(brickFragShader);
		glDeleteVertexArrays(1, &brickVAO);
		glDeleteBuffers(1, &brickMeshBuffer);
	}

	void update(double dt, int32_t size, Position* const pos, Velocity* const vel)
	{
		int32_t rectCount = rects.size();
		for (int i = rectCount - 1; i >= 0; i--)
		{
			Rect& r = rects[i];
			int& hp = hps[i];
			for (int32_t i = 0; i < size; i++)
			{
				Position& p = pos[i];
				Velocity& v = vel[i];

				if (abs((p.x + v.x * dt) - r.x) > (r.hHeight + 0.01f))
				{
					continue;
				}
				if (abs((p.y + v.y * dt) - r.y) > (r.hWidth + 0.01f))
				{
					continue;
				}

				hp--;

				if (p.y < r.y - (r.hHeight + 0.01f))
				{
					p.y = r.y - (r.hHeight + 0.01f);
					v.y *= -1;
					continue;
				}
				if (p.x < r.x - (r.hWidth + 0.01f))
				{
					p.x = r.x - (r.hWidth + 0.01f);
					v.x *= -1;
					continue;
				}
				if (p.x > r.x + (r.hWidth + 0.01f))
				{
					p.x = r.x + (r.hWidth + 0.01f);
					v.x *= -1;
					continue;
				}
				if (p.y > r.y + (r.hHeight + 0.01f))
				{
					p.y = r.y + (r.hHeight + 0.01f);
					v.y *= -1;
					continue;
				}
			}
			if (hp <= 0)
			{
				for (int rId = i; rId < rectCount - 1; rId++)
				{
					rects[rId] = rects[rId + 1];
					hps[rId] = hps[rId + 1];
				}
				rects.pop_back();
				hps.pop_back();
				rectCount--;
				continue;
			}
		}
	}

	void addBox(Rect rect, int hp)
	{
		rects.push_back(rect);
		hps.push_back(hp);
	}

	void drawBoxes()
	{
		glUseProgram(brickShaderProg);
		int32_t quadCount = rects.size();
		glUniform4fv(brickRectLoc, quadCount, reinterpret_cast<GLfloat*>(rects.data()));
		glUniform1iv(brickHpLoc, quadCount, reinterpret_cast<GLint*>(hps.data()));
		glBindVertexArray(brickVAO);
		glDrawArraysInstanced(GL_QUADS, 0, 4, quadCount);
		glBindVertexArray(0);
	}
};

#endif
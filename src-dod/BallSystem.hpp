#ifndef BALLSYSTEM_HPP
#define BALLSYSTEM_HPP

#include <glad/glad.h>
#include <ravine/ecs.h>

#include "io.hpp"
#include "shader.hpp"

#include "position.h"

using namespace rv;

static const float scale = 1e-2;
static const struct
{
	float x, y;
	float r, g, b;
} ballMesh[24] =
{
	// Right
	{ +0.0f * scale, +0.0f * scale, 1.f, 0.f, 0.f },
	{ +1.0f * scale, -0.4f * scale, 0.f, 0.f, 0.f },
	{ +1.0f * scale, +0.4f * scale, 0.f, 0.f, 0.f },
	// Top-Right
	{ +0.0f * scale, +0.0f * scale, 1.f, 0.f, 0.f },
	{ +1.0f * scale, +0.4f * scale, 0.f, 0.f, 0.f },
	{ +0.4f * scale, +1.0f * scale, 0.f, 0.f, 0.f },
	// Top
	{ +0.0f * scale, +0.0f * scale, 1.f, 0.f, 0.f },
	{ +0.4f * scale, +1.0f * scale, 0.f, 0.f, 0.f },
	{ -0.4f * scale, +1.0f * scale, 0.f, 0.f, 0.f },
	// Top-Left
	{ +0.0f * scale, +0.0f * scale, 1.f, 0.f, 0.f },
	{ -0.4f * scale, +1.0f * scale, 0.f, 0.f, 0.f },
	{ -1.0f * scale, +0.4f * scale, 0.f, 0.f, 0.f },
	// Left
	{ +0.0f * scale, +0.0f * scale, 1.f, 0.f, 0.f },
	{ -1.0f * scale, +0.4f * scale, 0.f, 0.f, 0.f },
	{ -1.0f * scale, -0.4f * scale, 0.f, 0.f, 0.f },
	// Bottom-Left
	{ +0.0f * scale, +0.0f * scale, 1.f, 0.f, 0.f },
	{ -1.0f * scale, -0.4f * scale, 0.f, 0.f, 0.f },
	{ -0.4f * scale, -1.0f * scale, 0.f, 0.f, 0.f },
	// Bottom
	{ +0.0f * scale, +0.0f * scale, 1.f, 0.f, 0.f },
	{ -0.4f * scale, -1.0f * scale, 0.f, 0.f, 0.f },
	{ +0.4f * scale, -1.0f * scale, 0.f, 0.f, 0.f },
	// Bottom-Right
	{ +0.0f * scale, +0.0f * scale, 1.f, 0.f, 0.f },
	{ +0.4f * scale, -1.0f * scale, 0.f, 0.f, 0.f },
	{ +1.0f * scale, -0.4f * scale, 0.f, 0.f, 0.f }
};

class BallSystem : public BaseSystem<Position>
{
private:
	GLuint ballVertShader;
	GLuint ballFragShader;
	GLuint ballShaderProg;
	GLuint ballVAO;
	GLuint posBuffer;
	GLuint ballMeshBuffer;

public:
	BallSystem(GLint maxBalls)
	{
		// Load Vertex Shader
		char* vertex_shader_text;
		rvLoadFile("../shaders/vert_ball_instanced.shader", vertex_shader_text, true);
		ballVertShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(ballVertShader, 1, &vertex_shader_text, NULL);
		glCompileShader(ballVertShader);
		if (!checkShaderCompile(ballVertShader))
		{
			getchar();
			return;
		}
		// Load Fragment Shader
		char* fragment_shader_text;
		rvLoadFile("../shaders/frag_col.shader", fragment_shader_text, true);
		ballFragShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(ballFragShader, 1, &fragment_shader_text, NULL);
		glCompileShader(ballFragShader);
		if (!checkShaderCompile(ballFragShader))
		{
			getchar();
			return;
		}
		// Create Pipeline Program
		ballShaderProg = glCreateProgram();
		glAttachShader(ballShaderProg, ballVertShader);
		glAttachShader(ballShaderProg, ballFragShader);
		glLinkProgram(ballShaderProg);
		if (!checkProgramLink(ballShaderProg))
		{
			getchar();
			return;
		}

		// Setup Array Buffer
		glGenVertexArrays(1, &ballVAO);
		glBindVertexArray(ballVAO);
		// Load Triangle Buffer
		glGenBuffers(1, &ballMeshBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, ballMeshBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ballMesh), ballMesh, GL_STATIC_DRAW);
		// Create Positions Buffer
		glGenBuffers(1, &posBuffer);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, posBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Position) * maxBalls, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, posBuffer);
		// Setup Bindings
		GLint vpos_location = glGetAttribLocation(ballShaderProg, "vertPos");
		GLint vcol_location = glGetAttribLocation(ballShaderProg, "vertCol");
		glEnableVertexAttribArray(vpos_location);
		glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
			sizeof(ballMesh[0]), (void*)0);
		glEnableVertexAttribArray(vcol_location);
		glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
			sizeof(ballMesh[0]), (void*)(sizeof(float) * 2));
		// Unbind Buffers
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	~BallSystem()
	{
		glDeleteProgram(ballShaderProg);
		glDeleteShader(ballVertShader);
		glDeleteShader(ballFragShader);
		glDeleteVertexArrays(1, &ballVAO);
		glDeleteBuffers(1, &posBuffer);
		glDeleteBuffers(1, &ballMeshBuffer);
	}

	void update(double deltaTime, int32_t offset, int32_t size, int32_t batchSize, Position* const pos) final
	{
		// Update Chunk Data
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, posBuffer);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset * sizeof(Position), batchSize * sizeof(Position), pos);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	void drawBalls(int32_t trisCount)
	{
		glUseProgram(ballShaderProg);
		glBindVertexArray(ballVAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 24, trisCount);
		glBindVertexArray(0);
	}
};

#endif
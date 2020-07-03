#pragma once

#include "shader.hpp"
#include "io.hpp"

#include "mouse.h"
#include "brick.h"

static const struct
{
	float x, y;
} brickMesh[4]{
	{ -1.f, -1.f },
	{ +1.f, -1.f },
	{ +1.f, +1.f },
	{ -1.f, +1.f }
};

class BrickFactory
{
private:
	GLuint brickVertShader;
	GLuint brickFragShader;
	GLuint brickShaderProg;
	GLuint brickMeshBuffer;
	GLuint brickVAO;
	GLint brickRectLoc = -1;
	GLint brickHpLoc = -1;

public:
	BrickFactory()
	{
		// Load Vertex Shader
		char* shaderData;
		rvLoadFile("../shaders/vert_brick.shader", shaderData, true);
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

	~BrickFactory()
	{
		glDeleteProgram(brickShaderProg);
		glDeleteShader(brickVertShader);
		glDeleteShader(brickFragShader);
		glDeleteVertexArrays(1, &brickVAO);
		glDeleteBuffers(1, &brickMeshBuffer);
	}

	Brick create(glm::vec4 rect)
	{
		return Brick(rect, 10'000, brickRectLoc, brickHpLoc, brickShaderProg, brickVAO);
	}
};
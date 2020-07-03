#pragma once

#include "shader.hpp"
#include "io.hpp"

#include "mouse.h"
#include "brick.h"

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

class BallFactory
{
private:
	GLuint ballVertShader;
	GLuint ballFragShader;
	GLuint ballShaderProg;
	GLuint ballMeshBuffer;
	GLuint ballVAO;
	GLint ballPosLoc;
	Mouse* mouse;

public:
	BallFactory(Mouse* mouse) : mouse(mouse)
	{
		// Load Vertex Shader
		char* vertex_shader_text;
		rvLoadFile("../shaders/vert_ball.shader", vertex_shader_text, true);
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
		// Get Uniform Location for position
		ballPosLoc = glGetUniformLocation(ballShaderProg, "pos");

		// Setup Array Buffer
		glGenVertexArrays(1, &ballVAO);
		glBindVertexArray(ballVAO);
		// Load Triangle Buffer
		glGenBuffers(1, &ballMeshBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, ballMeshBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ballMesh), ballMesh, GL_STATIC_DRAW);
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

	~BallFactory()
	{
		glDeleteProgram(ballShaderProg);
		glDeleteShader(ballVertShader);
		glDeleteShader(ballFragShader);
		glDeleteVertexArrays(1, &ballVAO);
		glDeleteBuffers(1, &ballMeshBuffer);
	}

	Ball create(glm::vec2 pos, glm::vec2 vel)
	{
		return Ball(scale, pos, vel, ballPosLoc, ballShaderProg, ballVAO, mouse);
	}
};
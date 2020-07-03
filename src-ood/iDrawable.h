#pragma once
#include <glad/glad.h>

class IDrawable
{
protected:
	GLuint program;
	GLuint vao;

public:
	IDrawable() = delete;
	IDrawable(GLuint program, GLuint vao) : program(program), vao(vao) {};
	virtual void draw() = 0;
};
#pragma once

#include <glfw/glfw3.h>

struct Mouse
{
	double x, y;
	GLFWwindow* window;

	inline Mouse(GLFWwindow* window, int width, int height) : window(window)
	{
		Update(width, height);
	}

	inline void Update(int width, int height)
	{
		glfwGetCursorPos(window, &x, &y);
		x = (x / (double)width) * 2.0 - 1.0;
		y = (y / (double)height) * -2.0 + 1.0;
	}
};


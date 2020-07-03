#include <stdlib.h>
#include <vector>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fmt/format.h>
#include <fmt/printf.h>

// Auxiliar Include
#include "debug.hpp"
#include "mouse.h"

#include "ballFactory.hpp"
#include "brickFactory.hpp"

using namespace fmt;
using namespace rv;

using std::vector;

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void)
{

#pragma region Setup
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(1920, 1080, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	//Check OpenGL version
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	fprintf(stdout, "Renderer: %s\n", renderer);
	fprintf(stdout, "OpenGL version supported %s\n", version);

	// Render Options
	glfwSwapInterval(0);

	// Interaction Options
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

	// Set Window Options
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float ratio = width / (float)height;
	glViewport(0, 0, width, height);
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	Mouse mouse(window, width, height);
#pragma endregion

	glm::vec2 spawnPos(0, -0.75);
	BallFactory ballFactory(&mouse);
	vector<Ball> ballList;

	BrickFactory brickFactory;
	vector<Brick> brickList;
	for (int bX = -8; bX < 9; bX++)
	{
		for (int bY = 0; bY < 9; bY++)
		{
			float pX = (bX / 10.0) * 1.02;
			float pY = (bY / 10.0) * 1.02;
			brickList.push_back(brickFactory.create(glm::vec4(pX, pY, 0.1, 0.1)));
		}
	}

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glfwGetFramebufferSize(window, &width, &height);
		if (width / (float)height != ratio)
		{
			glViewport(0, 0, width, height);
			ratio = width / (float)height;
		}
		mouse.Update(width, height);
		static double lastTime = glfwGetTime() - 0.016;
		double time = glfwGetTime();
		double deltaTime = time - lastTime;
		lastTime = time;

		static int32_t entityCount = 0;
		static string title;
		static size_t frameCount = 0;
		if (frameCount % 15 == 0)
		{
			title = format("FPS ({0:.3f}ms) - Entities ({1})", deltaTime * 1000.0, entityCount);
			glfwSetWindowTitle(window, title.c_str());
		}
		frameCount++;
		//ᴧᴧᴧᴧᴧ(Frame-Start)ᴧᴧᴧᴧᴧ

		//======(App-Logic)======
		
		// Instantiate balls
		if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS))
		{
			float difX = mouse.x - spawnPos.x;
			float difY = mouse.y - spawnPos.y;
			float mag = sqrt(pow(difX, 2) + pow(difY, 2));
			difX /= mag;
			difY /= mag;
			if (entityCount < 100'000)
			{
				ballList.push_back(ballFactory.create(spawnPos, glm::vec2(difX, difY)));
				entityCount++;
			}
		}

		// Update bricks
		int32_t rectCount = brickList.size();
		for (int32_t i = rectCount - 1; i >= 0; i--)
		{
			brickList[i].update(ballList, deltaTime);
			if (brickList[i].hp <= 0)
			{
				rvDebugLogVar("rect " + std::to_string(i) + " destroyed!");
				for (int rId = i; rId < rectCount - 1; rId++)
				{
					brickList[rId] = brickList[rId + 1];
				}
				brickList.pop_back();
			}
		}
		// Update balls
		for (size_t i = 0; i < ballList.size(); i++)
		{
			ballList[i].update(deltaTime);
		}
		//======(App-Logic)======

		//=======(Drawing)=======
		for (size_t i = 0; i < brickList.size(); i++)
		{
			brickList[i].draw();
		}
		for (size_t i = 0; i < ballList.size(); i++)
		{
			ballList[i].draw();
		}
		//=======(Drawing)=======

		//VVVVVV(Frame-End)VVVVVV
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
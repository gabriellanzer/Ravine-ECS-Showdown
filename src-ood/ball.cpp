#include "ball.h"

Ball::Ball(float radius, glm::vec2 pos, glm::vec2 vel, GLint posLoc, GLuint program, GLuint vao, Mouse* mouse) :
	radius(radius), mouse(mouse), posLoc(posLoc), IDrawable(program, vao), RigidBody(pos, vel)
{

}

void Ball::draw()
{
	glUseProgram(program);
	glBindVertexArray(vao);
	glUniform2f(posLoc, position.x, position.y);
	glDrawArrays(GL_TRIANGLES, 0, 24);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Ball::update(double deltaTime)
{
    glm::vec2 mPos(mouse->x, mouse->y);

    if (glfwGetMouseButton(mouse->window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
    {
        float dist = glm::distance(position, mPos);
        if (dist < 0.2f)
        {
            float vMag = glm::length(velocity);
            accelerate((mPos - position) * pow(1.0f - dist * 0.5f, 3.0f) * 100.0f, deltaTime);
        }
    }

	RigidBody::update(deltaTime);
}
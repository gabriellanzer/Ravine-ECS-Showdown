#ifndef ATTRACTIONSYSTEM_HPP
#define ATTRACTIONSYSTEM_HPP

#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <ravine/ecs.h>

#include "mouse.h"
#include "position.h"
#include "velocity.h"

using namespace rv;

class AttractionSystem : public BaseSystem<Position, Velocity>
{
private:
    GLFWwindow* window;
    Mouse* mouse;

public:
    constexpr AttractionSystem(GLFWwindow* window, Mouse* mouse) : window(window), mouse(mouse) {}

    void update(double deltaTime, int32_t size, Position* const pos, Velocity* const vel) final
    {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) != GLFW_PRESS)
        {
            return;
        }

        glm::vec2 mPos(mouse->x, mouse->y);
        for (int32_t i = 0; i < size; i++)
        {
            glm::vec2& p = *reinterpret_cast<glm::vec2*>(&pos[i]);
            float dist = glm::distance(p, mPos);
            if (dist > 0.2f)
                continue;

            glm::vec2& v = *reinterpret_cast<glm::vec2*>(&vel[i]);
            float vMag = glm::length(v);
            v += (mPos - p) * pow(1.0f - dist*0.5f, 3.0f) * 100.0f * (float)deltaTime;
        }
    }
};

#endif
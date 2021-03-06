#ifndef MOVEMENTSYSTEM_HPP
#define MOVEMENTSYSTEM_HPP

#include "ravine/ecs.h"
#include "position.h"
#include "velocity.h"

using namespace rv;

class MovementSystem : public BaseSystem<Velocity, Position>
{
public:
    void update(double deltaTime, int32_t size, Velocity* const vel, Position* const pos) final
    {
        for (int32_t i = 0; i < size; i++)
        {
            pos[i].x += vel[i].x * deltaTime;
            pos[i].y += vel[i].y * deltaTime;
        }
    }
};

#endif
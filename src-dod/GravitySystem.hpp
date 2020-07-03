#ifndef GRAVITYSYSTEM_HPP
#define GRAVITYSYSTEM_HPP

#include "ravine/ecs.h"
#include "velocity.h"

using namespace rv;

class GravitySystem : public BaseSystem<Velocity>
{
    const float gravity;

public:
    constexpr GravitySystem(float gravityVal) : gravity(gravityVal)
    {

    }

    void update(double deltaTime, int32_t size, Velocity* const vel) final
    {
        for (int32_t i = 0; i < size; i++)
        {
            vel[i].y += gravity * deltaTime;
        }
    }
};

#endif
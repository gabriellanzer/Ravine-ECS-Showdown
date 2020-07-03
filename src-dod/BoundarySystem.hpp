#ifndef BOUNDARYSYSTEM_HPP
#define BOUNDARYSYSTEM_HPP

#include <math.h>
#include "ravine/ecs.h"
#include "position.h"
#include "velocity.h"

using namespace rv;

class BoundarySystem : public BaseSystem<Position, Velocity>
{

public:
    void update(double deltaTime, int32_t size, Position* const pos, Velocity* const vel) final
    {
        for (int32_t i = 0; i < size; i++)
        {
            Position& p = pos[i];
            Velocity& v = vel[i];

            float absX = abs(p.x);
            float absY = abs(p.y);
            if (absX > 1.0f)
            {
                v.x *= -0.9;
                if (p.x < 0)
                {
                    p.x = -1.0;
                }
                else
                {
                    p.x = 1.0;
                }
            }

            if (absY > 1.0f)
            {
                v.y *= -0.9;
                if (p.y < 0)
                {
                    p.y = -1.0;
                }
                else
                {
                    p.y = 1.0;
                }
            }
        }
    }
};

#endif
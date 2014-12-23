#pragma once
#include <memory>
#include <vector>
#include <d3d9.h>

namespace games {
    typedef struct Fog {
        static Fog Disabled;

        Fog(float minDistance, float maxDistance);
        ~Fog();

        bool enabled = false;
        float minDistance = 0;
        float maxDistance = 0;
        D3DCOLOR color;

    private:
        Fog();
    } Fog;
}

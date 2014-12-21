#pragma once

#include "GameObject.h"

namespace games {

    class Container : public GameObject<Container>
    {
    public:
        Container() { }
        ~Container() { }
    };
}
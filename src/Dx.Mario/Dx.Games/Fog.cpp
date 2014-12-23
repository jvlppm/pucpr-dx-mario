#include "Fog.h"

using namespace games;

Fog Fog::Disabled = Fog();

Fog::Fog(float minDistance, float maxDistance) {
    this->minDistance = minDistance;
    this->maxDistance = maxDistance;
    this->enabled = true;
    this->color = D3DCOLOR_RGBA(150, 150, 150, 255);
}

Fog::Fog() {

}

Fog::~Fog() {

}

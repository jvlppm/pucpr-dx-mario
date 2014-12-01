#include "Resources.h"

using namespace mage;
using namespace games;

Resources::TexMap Resources::textures = {};
std::shared_ptr<ResourceLoader> Resources::loader = nullptr;
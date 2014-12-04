#include "Resources.h"

using namespace mage;
using namespace games;

Resources::TexMap Resources::textures = {};
Resources::MeshMap Resources::meshes = {};
Resources::EffectMap Resources::effects = {};
std::shared_ptr<IResourceLoader> Resources::loader = nullptr;
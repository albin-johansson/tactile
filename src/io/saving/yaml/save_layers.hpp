#pragma once

#include <yaml-cpp/yaml.h>

#include <entt.hpp>    // registry, entity
#include <filesystem>  // path

namespace Tactile::IO {

void SaveLayers(YAML::Emitter& emitter,
                const entt::registry& registry,
                const std::filesystem::path& dir);

}  // namespace Tactile::IO

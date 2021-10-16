#pragma once

#include <yaml-cpp/yaml.h>

#include <entt.hpp>    // registry, entity
#include <filesystem>  // path

namespace Tactile::IO {

void SaveProperties(YAML::Emitter& emitter,
                    const entt::registry& registry,
                    entt::entity entity,
                    const std::filesystem::path& dir);

}  // namespace Tactile::IO
#pragma once

#include <string>  // string
#include <vector>  // vector

#include <tactile_def.hpp>

#include <entt/entt.hpp>  // entity

namespace Tactile {

struct PropertyContext final {
  ContextID id{};
  std::string name;
  std::vector<entt::entity> properties;
  // TODO consider including component entities here? (and rename to Context?)
};

struct ActivePropertyContext final {
  entt::entity entity{entt::null};
};

}  // namespace Tactile

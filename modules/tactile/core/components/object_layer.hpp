#pragma once

#include <vector>  // vector

#include <entt/entt.hpp>  // entity

namespace Tactile {

struct ObjectLayer final
{
  std::vector<entt::entity> objects;
};

}  // namespace Tactile
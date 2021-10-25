#pragma once

#include <entt.hpp>  // entity
#include <vector>    // vector

namespace Tactile {

struct ObjectLayer final
{
  std::vector<entt::entity> objects;
};

}  // namespace Tactile
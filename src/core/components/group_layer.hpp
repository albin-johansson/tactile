#pragma once

#include <entt.hpp>  // entity
#include <vector>    // vector

namespace Tactile {

struct GroupLayer final
{
  std::vector<entt::entity> layers;
};

}  // namespace Tactile

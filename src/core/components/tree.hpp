#pragma once

#include <entt.hpp>  // entity
#include <vector>    // vector

namespace Tactile {

struct TreeNode final
{
  std::vector<entt::entity> children;
};

struct TreeLeaf final
{};

}  // namespace Tactile
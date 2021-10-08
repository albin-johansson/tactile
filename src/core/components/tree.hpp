#pragma once

#include <entt.hpp>  // entity
#include <vector>    // vector

#include "common/ints.hpp"

namespace Tactile {

struct TreeNode final
{
  usize index{};                       ///< Local index, i.e. relative to tree siblings.
  std::vector<entt::entity> children;  ///< All associated child nodes.
};

struct TreeLeaf final
{};

}  // namespace Tactile
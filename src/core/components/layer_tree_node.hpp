#pragma once

#include <entt.hpp>  // entity
#include <vector>    // vector

#include "common/ints.hpp"

namespace Tactile {

struct LayerTreeNode final
{
  usize index{};                       ///< Local index, i.e. relative to tree siblings.
  std::vector<entt::entity> children;  ///< All associated child nodes.
};

}  // namespace Tactile
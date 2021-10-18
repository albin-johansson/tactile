#pragma once

#include <vector>  // vector

#include <entt.hpp>  // entity

#include <tactile-base/tactile_std.hpp>

namespace Tactile {

struct LayerTreeNode final
{
  usize index{};                       ///< Local index, i.e. relative to tree siblings.
  std::vector<entt::entity> children;  ///< All associated child nodes.
};

}  // namespace Tactile
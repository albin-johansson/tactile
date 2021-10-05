#include "tree_system.hpp"

#include <cassert>  // assert

#include "core/components/parent.hpp"
#include "core/components/tree.hpp"

namespace Tactile::Sys::Tree {
namespace {}  // namespace

auto GetSiblingCount(const entt::registry& registry, const entt::entity entity) -> usize
{
  assert(entity != entt::null);
  assert(registry.all_of<Parent>(entity));

  const auto& parent = registry.get<Parent>(entity);

  if (parent.entity == entt::null) {
    usize count = 0;

    for (auto&& [otherEntity, otherNode, otherParent] :
         registry.view<TreeNode, Parent>().each())
    {
      if (otherEntity != entity && otherParent.entity == entt::null) {
        ++count;
      }
    }

    return count;
  }
  else {
    const auto& parentNode = registry.get<TreeNode>(parent.entity);
    return parentNode.children.size() - 1;  // Exclude the queried layer
  }
}

auto GetChildrenCount(const entt::registry& registry, const entt::entity entity) -> usize
{
  assert(entity != entt::null);

  if (const auto* node = registry.try_get<TreeNode>(entity)) {
    usize count = node->children.size();

    for (const auto child : node->children) {
      count += GetChildrenCount(registry, child);
    }

    return count;
  }
  else {
    return 0u;
  }
}

}  // namespace Tactile::Sys::Tree
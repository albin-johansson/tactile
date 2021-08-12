#ifndef RUNE_ECS_ECS_UTILS_HPP
#define RUNE_ECS_ECS_UTILS_HPP

#include <entt.hpp>  // registry, entity

#include "../core/iterable.hpp"

namespace rune {

/// \addtogroup ecs
/// \{

/**
 * \brief Destroys all entities in a container and subsequently clears the container.
 *
 * \tparam T the type of the container.
 *
 * \param registry the registry that the entities belongs to.
 * \param container the container that will be cleared.
 *
 * \since 0.1.0
 */
template <iterable<entt::entity> T>
void destroy_and_clear(entt::registry& registry, T& container)
{
  for (const auto entity : container)
  {
    registry.destroy(entity);
  }

  container.clear();
}

/// \} End of group ecs

}  // namespace rune

#endif  // RUNE_ECS_ECS_UTILS_HPP

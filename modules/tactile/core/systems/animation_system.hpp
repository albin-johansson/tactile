#pragma once

#include <entt/entt.hpp>  // registry

namespace tactile::sys {

/// \name Animation system
/// \{

/**
 * \brief Updates all animations in the registry.
 *
 * \param registry the registry that will be updated.
 *
 * \see Animation
 */
void UpdateAnimations(entt::registry& registry);

/// \} End of animation system

}  // namespace tactile::sys

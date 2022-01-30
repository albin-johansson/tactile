#pragma once

#include <entt/entt.hpp>

namespace tactile::sys {

/// \name Animation system
/// \{

/**
 * \brief Updates all animations in a registry.
 *
 * \ingroup systems
 *
 * \param registry the document registry.
 */
void update_animations(entt::registry& registry);

/// \} End of animation system

}  // namespace tactile::sys

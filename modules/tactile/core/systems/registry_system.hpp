#pragma once

#include <entt/entt.hpp>

namespace tactile::sys {

/**
 * \brief Creates a registry with the expected context variables.
 *
 * \details The created registry will feature the following context variables:
 * - `MapInfo`
 * - `ActiveLayer`
 * - `ActiveTileset`
 * - `ActivePropertyContext`
 * - `ActiveTool`
 * - `ActiveObject`
 * - `Viewport`
 * - `attribute_context`
 * - `TilesetContext`
 * - `MousePos`
 *
 * \return a registry.
 *
 * \since 0.2.0
 */
[[nodiscard]] auto MakeRegistry() -> entt::registry;

}  // namespace tactile::sys

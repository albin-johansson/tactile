#pragma once

#include <entt/entt.hpp>

namespace tactile::sys {

/**
 * \brief Creates a registry with the expected context variables.
 *
 * \ingroup systems
 *
 * \details The created registry will feature the following context variables:
 * - `MapInfo`
 * - `TilesetContext`
 * - `MousePos`
 * - `Viewport`
 * - `attribute_context`
 * - `ActiveLayer`
 * - `ActiveTileset`
 * - `active_attribute_context`
 * - `ActiveTool`
 * - `ActiveObject`
 *
 * \return a document registry.
 */
[[nodiscard]] auto make_document_registry() -> entt::registry;

}  // namespace tactile::sys

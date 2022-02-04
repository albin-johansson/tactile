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
 * - `Viewport`
 * - `tileset_context`
 * - `attribute_context`
 * - `active_layer`
 * - `active_tileset`
 * - `active_attribute_context`
 * - `active_tool`
 * - `active_object`
 *
 * \return a document registry.
 */
[[nodiscard]] auto make_document_registry() -> entt::registry;

}  // namespace tactile::sys

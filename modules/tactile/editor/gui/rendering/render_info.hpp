#pragma once

#include <centurion.hpp>  // frect
#include <entt/entt.hpp>  // registry
#include <imgui.h>

#include "core/region.hpp"

namespace Tactile {

struct CanvasInfo;

/// \addtogroup rendering
/// \{

/**
 * \brief Provides useful information about a rendering context.
 *
 * \see `GetRenderInfo()`
 */
struct RenderInfo final {
  Region bounds;           ///< The area of the map that should be rendered.
  cen::FRect bounds_rect;  ///< Same as bounds but as a rectangle.

  ImVec2 map_position{};  ///< Absolute screen position of the map.
  ImVec2 grid_size{};     ///< Graphical tile size.
  ImVec2 tile_size{};     ///< Logical tile size.
  ImVec2 ratio{};         ///< Graphical tile size divided by logical tile size.

  float row_count{};  ///< Total amount of rows.
  float col_count{};  ///< Total amount of columns.
};

/**
 * \brief Returns information about a rendering context.
 *
 * \param registry the currently active registry.
 * \param canvas information about the target canvas.
 *
 * \return information about the rendering context.
 */
[[nodiscard]] auto GetRenderInfo(const entt::registry& registry, const CanvasInfo& canvas)
    -> RenderInfo;

/// \} End of group rendering

}  // namespace Tactile

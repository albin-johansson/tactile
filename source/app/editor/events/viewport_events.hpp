#pragma once

#include <entt/entt.hpp>

namespace tactile {

/// \addtogroup events
/// \{

struct CenterViewportEvent final
{};

struct ResetZoomEvent final
{};

struct DecreaseZoomEvent final
{};

struct IncreaseZoomEvent final
{};

struct OffsetViewportEvent final
{
  float dx{};
  float dy{};
};

/**
 * \brief Offset a viewport that cannot exceed the bounds of the shown content.
 */
struct OffsetBoundViewportEvent final
{
  entt::entity entity{entt::null};
  float dx{};
  float dy{};
  float view_width{};
  float view_height{};
};

struct PanUpEvent final
{};

struct PanDownEvent final
{};

struct PanLeftEvent final
{};

struct PanRightEvent final
{};

/// \} End of group events

}  // namespace tactile
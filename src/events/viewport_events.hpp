#pragma once

namespace Tactile {

/// \addtogroup events
/// \{

struct CenterViewportEvent final
{};

struct ResetViewportZoomEvent final
{};

struct DecreaseViewportZoomEvent final
{};

struct IncreaseViewportZoomEvent final
{};

struct OffsetViewportEvent final
{
  float dx{};
  float dy{};
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

}  // namespace Tactile
#pragma once

namespace Tactile {

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
// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile::core {

/**
 * Event for moving a viewport in the current document.
 */
struct OffsetViewportEvent final
{
  /** The target viewport. */
  EntityID viewport_entity;

  /** The offset to apply to the viewport position. */
  Float2 delta;
};

/**
 * Event for changing the size a viewport in the current document.
 */
struct UpdateViewportSizeEvent final
{
  /** The target viewport. */
  EntityID viewport_entity;

  /** The new viewport size. */
  Float2 new_size;
};

/**
 * Event for changing the limits of a viewport in the current document.
 */
struct UpdateViewportLimitsEvent final
{
  /** The target viewport. */
  EntityID viewport_entity;

  /** The new minimum position. */
  Float2 min_pos;

  /** The new maximum position. */
  Float2 max_pos;
};

/**
 * Event for increasing the zoom of a viewport in the current document.
 */
struct IncreaseViewportZoomEvent final
{
  /** The target viewport. */
  EntityID viewport_entity;
};

/**
 * Event for decreasing the zoom of a viewport in the current document.
 */
struct DecreaseViewportZoomEvent final
{
  /** The target viewport. */
  EntityID viewport_entity;
};

/**
 * Event for resetting the zoom of a viewport in the current document.
 */
struct ResetViewportZoomEvent final
{
  /** The target viewport. */
  EntityID viewport_entity;
};

/**
 * Event for centering a viewport in the current document.
 */
struct CenterViewportEvent final
{
  /** The target viewport. */
  EntityID viewport_entity;

  /** The size of the underlying content. */
  Float2 content_size;
};

/**
 * Event for panning a viewport upwards in the current document.
 */
struct PanViewportUpEvent final
{
  /** The target viewport. */
  EntityID viewport_entity;
};

/**
 * Event for panning a viewport downwards in the current document.
 */
struct PanViewportDownEvent final
{
  /** The target viewport. */
  EntityID viewport_entity;
};

/**
 * Event for panning a viewport leftwards in the current document.
 */
struct PanViewportLeftEvent final
{
  /** The target viewport. */
  EntityID viewport_entity;
};

/**
 * Event for panning a viewport rightwards in the current document.
 */
struct PanViewportRightEvent final
{
  /** The target viewport. */
  EntityID viewport_entity;
};

}  // namespace tactile::core

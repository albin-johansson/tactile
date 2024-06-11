// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

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

}  // namespace tactile

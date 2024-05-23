// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * An event for undoing the most recent command.
 *
 * \ingroup Event
 */
struct UndoEvent final
{};

/**
 * An event for redoing the most recently reverted command.
 *
 * \ingroup Event
 */
struct RedoEvent final
{};

/**
 * An event for enabling the stamp tool.
 *
 * \ingroup Event
 */
struct EnableStampToolEvent final
{};

/**
 * An event for enabling the eraser tool.
 *
 * \ingroup Event
 */
struct EnableEraserToolEvent final
{};

/**
 * An event for enabling the bucket fill tool.
 *
 * \ingroup Event
 */
struct EnableBucketToolEvent final
{};

/**
 * An event for enabling the object selector tool.
 *
 * \ingroup Event
 */
struct EnableSelectorToolEvent final
{};

/**
 * An event for enabling the rectangle tool.
 *
 * \ingroup Event
 */
struct EnableRectangleToolEvent final
{};

/**
 * An event for enabling the ellipse tool.
 *
 * \ingroup Event
 */
struct EnableEllipseToolEvent final
{};

/**
 * An event for enabling the point tool.
 *
 * \ingroup Event
 */
struct EnablePointToolEvent final
{};

}  // namespace tactile

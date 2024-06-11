// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Event for undoing the most recent command.
 */
struct UndoEvent final
{};

/**
 * Event for redoing the most recently reverted command.
 */
struct RedoEvent final
{};

/**
 * Event for enabling the stamp tool.
 */
struct EnableStampToolEvent final
{};

/**
 * Event for enabling the eraser tool.
 */
struct EnableEraserToolEvent final
{};

/**
 * Event for enabling the bucket fill tool.
 */
struct EnableBucketToolEvent final
{};

/**
 * Event for enabling the object selector tool.
 */
struct EnableSelectorToolEvent final
{};

/**
 * Event for enabling the rectangle tool.
 */
struct EnableRectangleToolEvent final
{};

/**
 * Event for enabling the ellipse tool.
 */
struct EnableEllipseToolEvent final
{};

/**
 * Event for enabling the point tool.
 */
struct EnablePointToolEvent final
{};

}  // namespace tactile

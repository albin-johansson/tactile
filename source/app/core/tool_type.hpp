#pragma once

namespace tactile {

/**
 * \brief Represents the various available mouse tool types.
 *
 * \ingroup core
 */
enum class tool_type
{
  none,
  stamp,
  eraser,
  bucket,
  object_selection,
  rectangle,
  ellipse,
  point
};

}  // namespace tactile

#pragma once

namespace tactile {

/**
 * \enum MouseToolType
 *
 * \brief Provides identifiers for the various mouse tools.
 *
 * \since 0.1.0
 *
 * \headerfile mouse_tool_type.hpp
 */
enum class MouseToolType
{
  None,    ///< Represents no mouse tool.
  Stamp,   ///< Represents the stamp tool.
  Bucket,  ///< Represents the bucket fill tool.
  Eraser   ///< Represents the eraser tool.
};

}  // namespace tactile

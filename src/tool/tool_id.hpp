#pragma once

namespace tactile {

/**
 * \enum tool_id
 *
 * \brief Provides identifiers for the various mouse tools.
 *
 * \since 0.1.0
 *
 * \headerfile tool_id.hpp
 */
enum class tool_id
{
  none,       ///< Represents no mouse tool.
  stamp,      ///< Represents the stamp tool.
  bucket,     ///< Represents the bucket fill tool.
  eraser,     ///< Represents the eraser tool.
  rectangle,  ///< Represents the rectangle tool.
  find_same   ///< Represents the find-same tool.
};

}  // namespace tactile

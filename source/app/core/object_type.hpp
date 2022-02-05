#pragma once

#include <ostream>  // ostream

#include <magic_enum.hpp>

namespace tactile {

/**
 * \brief Represents the different available map object types.
 */
enum class object_type
{
  point,
  rect,
  ellipse
};

inline auto operator<<(std::ostream& stream, const object_type type) -> std::ostream&
{
  return stream << magic_enum::enum_name(type);
}

}  // namespace tactile
#ifndef RUNE_TMX_STAGGER_AXIS_HPP
#define RUNE_TMX_STAGGER_AXIS_HPP

#include <json.hpp>  // NLOHMANN_JSON_SERIALIZE_ENUM

namespace rune {

/// \addtogroup tmx
/// \{

enum class tmx_stagger_axis
{
  x,
  y
};

NLOHMANN_JSON_SERIALIZE_ENUM(tmx_stagger_axis,
                             {{tmx_stagger_axis::x, "x"}, {tmx_stagger_axis::y, "y"}})

/// \} End of group tmx

}  // namespace rune

#endif  // RUNE_TMX_STAGGER_AXIS_HPP

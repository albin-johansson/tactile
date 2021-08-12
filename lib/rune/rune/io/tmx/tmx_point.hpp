#ifndef RUNE_TMX_POINT_HPP
#define RUNE_TMX_POINT_HPP

#include "../../aliases/json_type.hpp"

namespace rune {

/// \addtogroup tmx
/// \{

struct tmx_point final
{
  float x{};  ///< The x-coordinate of the point.
  float y{};  ///< The y-coordinate of the point.
};

inline void from_json(const json_type& json, tmx_point& point)
{
  json.at("x").get_to(point.x);
  json.at("y").get_to(point.y);
}

/// \} End of group tmx

}  // namespace rune

#endif  // RUNE_TMX_POINT_HPP

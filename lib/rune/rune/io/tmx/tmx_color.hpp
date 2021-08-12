#ifndef RUNE_TMX_COLOR_HPP
#define RUNE_TMX_COLOR_HPP

#include <cassert>      // assert
#include <string>       // string
#include <string_view>  // string_view

#include "../../aliases/integers.hpp"
#include "../../aliases/json_type.hpp"
#include "core/from_string.hpp"

namespace rune {

/// \addtogroup tmx
/// \{

struct tmx_color final
{
  uint8 red{};
  uint8 green{};
  uint8 blue{};
  uint8 alpha{0xFF};

  [[nodiscard]] constexpr bool operator==(const tmx_color&) const noexcept = default;
};

/// \} End of group tmx

namespace tmx {

/// \addtogroup tmx
/// \{

inline constexpr tmx_color black{0, 0, 0, 0xFF};

[[nodiscard]] inline auto from_hex(const std::string_view str) -> uint8
{
  assert(str.size() == 2);
  return from_string<uint8>(str, 16).value();
}

[[nodiscard]] inline auto make_color(const std::string_view str) -> tmx_color
{
  assert(str.size() == 7 || str.size() == 9);
  assert(str.at(0) == '#');

  const auto noHash = str.substr(1);
  const auto length = noHash.size();

  tmx_color result;

  if (length == 8)
  {
    // ARGB
    result.alpha = from_hex(noHash.substr(0, 2));
    result.red = from_hex(noHash.substr(2, 2));
    result.green = from_hex(noHash.substr(4, 2));
    result.blue = from_hex(noHash.substr(6, 2));
  }
  else
  {
    // RGB
    result.red = from_hex(noHash.substr(0, 2));
    result.green = from_hex(noHash.substr(2, 2));
    result.blue = from_hex(noHash.substr(4, 2));
  }

  return result;
}

/// \} End of group tmx

}  // namespace tmx

inline void from_json(const json_type& json, tmx_color& color)
{
  color = tmx::make_color(json.get<std::string>());
}

}  // namespace rune

#endif  // RUNE_TMX_COLOR_TEST

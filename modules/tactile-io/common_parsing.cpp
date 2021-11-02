#include "common_parsing.hpp"

#include <tactile-base/from_string.hpp>

namespace Tactile::IO {

auto ParseColorRGB(const std::string_view rgb) -> std::optional<Color>
{
  if (rgb.size() != 7 || rgb.front() != '#') {
    return std::nullopt;
  }

  const auto noHash = rgb.substr(1);

  const auto rr = noHash.substr(0, 2);
  const auto gg = noHash.substr(2, 2);
  const auto bb = noHash.substr(4, 2);

  const auto red = FromString<uint8>(rr, 16);
  const auto green = FromString<uint8>(gg, 16);
  const auto blue = FromString<uint8>(bb, 16);

  if (red && green && blue) {
    return Color{*red, *green, *blue, 0xFF};
  }
  else {
    return std::nullopt;
  }
}

auto ParseColorRGBA(const std::string_view rgba) -> std::optional<Color>
{
  if (rgba.size() != 9 || rgba.front() != '#') {
    return std::nullopt;
  }

  const auto noHash = rgba.substr(1);

  const auto rr = noHash.substr(0, 2);
  const auto gg = noHash.substr(2, 2);
  const auto bb = noHash.substr(4, 2);
  const auto aa = noHash.substr(6, 2);

  const auto red = FromString<uint8>(rr, 16);
  const auto green = FromString<uint8>(gg, 16);
  const auto blue = FromString<uint8>(bb, 16);
  const auto alpha = FromString<uint8>(aa, 16);

  if (red && green && blue && alpha) {
    return Color{*red, *green, *blue, *alpha};
  }
  else {
    return std::nullopt;
  }
}

auto ParseColorARGB(const std::string_view argb) -> std::optional<Color>
{
  if (argb.size() != 9 || argb.front() != '#') {
    return std::nullopt;
  }

  const auto noHash = argb.substr(1);

  const auto aa = noHash.substr(0, 2);
  const auto rr = noHash.substr(2, 2);
  const auto gg = noHash.substr(4, 2);
  const auto bb = noHash.substr(6, 2);

  const auto red = FromString<uint8>(rr, 16);
  const auto green = FromString<uint8>(gg, 16);
  const auto blue = FromString<uint8>(bb, 16);
  const auto alpha = FromString<uint8>(aa, 16);

  if (red && green && blue && alpha) {
    return Color{*red, *green, *blue, *alpha};
  }
  else {
    return std::nullopt;
  }
}

}  // namespace Tactile::IO
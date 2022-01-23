#include "common_parsing.hpp"

#include <tactile_stdlib.hpp>

namespace tactile::IO {

auto ParseColorRGB(const std::string_view rgb) -> std::optional<Color>
{
  if (rgb.size() != 7 || rgb.front() != '#') {
    return std::nullopt;
  }

  const auto noHash = rgb.substr(1);

  const auto rr = noHash.substr(0, 2);
  const auto gg = noHash.substr(2, 2);
  const auto bb = noHash.substr(4, 2);

  const auto red = FromString<uint8>(rr.data(), rr.size(), 16);
  const auto green = FromString<uint8>(gg.data(), gg.size(), 16);
  const auto blue = FromString<uint8>(bb.data(), bb.size(), 16);

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

  const auto red = FromString<uint8>(rr.data(), rr.size(), 16);
  const auto green = FromString<uint8>(gg.data(), gg.size(), 16);
  const auto blue = FromString<uint8>(bb.data(), bb.size(), 16);
  const auto alpha = FromString<uint8>(aa.data(), aa.size(), 16);

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

  const auto red = FromString<uint8>(rr.data(), rr.size(), 16);
  const auto green = FromString<uint8>(gg.data(), gg.size(), 16);
  const auto blue = FromString<uint8>(bb.data(), bb.size(), 16);
  const auto alpha = FromString<uint8>(aa.data(), aa.size(), 16);

  if (red && green && blue && alpha) {
    return Color{*red, *green, *blue, *alpha};
  }
  else {
    return std::nullopt;
  }
}

auto ParseAttributeType(const std::string_view type) -> std::optional<PropertyType>
{
  if (type == "string") {
    return PropertyType::String;
  }
  else if (type == "int") {
    return PropertyType::Integer;
  }
  else if (type == "float") {
    return PropertyType::Floating;
  }
  else if (type == "bool") {
    return PropertyType::Boolean;
  }
  else if (type == "color") {
    return PropertyType::Color;
  }
  else if (type == "object") {
    return PropertyType::Object;
  }
  else if (type == "file") {
    return PropertyType::File;
  }
  else {
    return std::nullopt;
  }
}

}  // namespace tactile::IO
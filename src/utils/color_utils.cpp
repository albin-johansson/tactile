#include "color_utils.hpp"

#include "aliases/ints.hpp"
#include "utils/from_string.hpp"

namespace Tactile {

auto ColorFromString(const std::string_view str) -> Maybe<cen::color>
{
  if (!str.starts_with('#'))
  {
    CENTURION_LOG_ERROR("ARGB string must start with '#' character! String: %s",
                        str.data());
    return nothing;
  }

  if (str.size() != 7 && str.size() != 9)
  {
    CENTURION_LOG_ERROR(
        "ARGB string must be either 7 or 9 characters long! Was actually: %u",
        str.size());
    return nothing;
  }

  const auto noHash = str.substr(1);
  const auto length = noHash.size();

  cen::color color;

  const auto fromHex = [](const std::string_view str) {
    return FromString<uint8>(str, 16).value();
  };

  if (length == 8)
  {
    // ARGB
    color.set_alpha(fromHex(noHash.substr(0, 2)));
    color.set_red(fromHex(noHash.substr(2, 2)));
    color.set_green(fromHex(noHash.substr(4, 2)));
    color.set_blue(fromHex(noHash.substr(6, 2)));
  }
  else
  {
    // RGB
    color.set_red(fromHex(noHash.substr(0, 2)));
    color.set_green(fromHex(noHash.substr(2, 2)));
    color.set_blue(fromHex(noHash.substr(4, 2)));
  }

  return color;
}

}  // namespace Tactile

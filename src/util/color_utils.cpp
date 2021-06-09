#include "color_utils.hpp"

namespace tactile {
namespace {

inline constexpr double red_factor = 0.299;
inline constexpr double green_factor = 0.587;
inline constexpr double blue_factor = 0.114;

}  // namespace

auto intensity_of(const QColor& color) -> double
{
  return (color.red() * red_factor) + (color.green() * green_factor) +
         (color.blue() * blue_factor);
}

auto is_bright(const QColor& color) -> bool
{
  return intensity_of(color) > color_intensity_threshold;
}

}  // namespace tactile

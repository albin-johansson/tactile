#include "color_utils.hpp"

namespace tactile {
namespace {

inline constexpr double red_factor = 0.299;
inline constexpr double green_factor = 0.587;
inline constexpr double blue_factor = 0.114;

}  // namespace

auto IntensityOf(const QColor& color) -> double
{
  return (color.red() * red_factor) + (color.green() * green_factor) +
         (color.blue() * blue_factor);
}

auto IsBright(const QColor& color) -> bool
{
  return IntensityOf(color) > color_intensity_threshold;
}

}  // namespace tactile

#include "color_utils.hpp"

namespace tactile {

auto intensity_of(const QColor& color) -> double
{
  return (color.red() * 0.299) + (color.green() * 0.587) +
         (color.blue() * 0.114);
}

}  // namespace tactile

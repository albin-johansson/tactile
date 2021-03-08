#pragma once

#include <QColor>  // QColor

namespace tactile {

[[nodiscard]] auto intensity_of(const QColor& color) -> double;

// An intensity threshold used to determine whether or not black text is
// required when using the color as a backdrop
[[nodiscard]] constexpr auto color_intensity_threshold() noexcept -> double
{
  return 186;
}

}  // namespace tactile

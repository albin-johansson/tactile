#include "centered_button.hpp"

#include "assert.hpp"
#include "button.hpp"
#include "editor/gui/alignment.hpp"

namespace tactile {

auto CenteredButton(const c_str text, const c_str tooltip) -> bool
{
  TACTILE_ASSERT(text);

  const auto width = GetStandardComponentWidth(text);
  AlignNextItemCenteredHorizontally(width);

  return Button(text, tooltip);
}

}  // namespace tactile

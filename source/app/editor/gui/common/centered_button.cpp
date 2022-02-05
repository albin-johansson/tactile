#include "centered_button.hpp"

#include "button.hpp"
#include "editor/gui/alignment.hpp"
#include "misc/assert.hpp"

namespace tactile {

auto CenteredButton(const c_str text, const c_str tooltip) -> bool
{
  TACTILE_ASSERT(text);

  const auto width = GetStandardComponentWidth(text);
  AlignNextItemCenteredHorizontally(width);

  return Button(text, tooltip);
}

}  // namespace tactile

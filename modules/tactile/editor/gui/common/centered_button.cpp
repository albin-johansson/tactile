#include "centered_button.hpp"

#include "assert.hpp"
#include "button.hpp"
#include "editor/gui/alignment.hpp"

namespace Tactile {

auto CenteredButton(const NotNull<CStr> text, const CStr tooltip) -> bool
{
  TACTILE_ASSERT(text);

  const auto width = GetStandardComponentWidth(text);
  AlignNextItemCenteredHorizontally(width);

  return Button(text, tooltip);
}

}  // namespace Tactile

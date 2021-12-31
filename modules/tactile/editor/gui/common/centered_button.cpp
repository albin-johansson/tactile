#include "centered_button.hpp"

#include <cassert>  // assert

#include "editor/gui/alignment.hpp"
#include "button.hpp"

namespace Tactile {

auto CenteredButton(const NotNull<CStr> text, const CStr tooltip) -> bool
{
  assert(text);

  const auto width = GetStandardComponentWidth(text);
  AlignNextItemCenteredHorizontally(width);

  return Button(text, tooltip);
}

}  // namespace Tactile

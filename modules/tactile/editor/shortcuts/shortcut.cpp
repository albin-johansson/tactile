#include "shortcut.hpp"

namespace Tactile {

void AShortcut::Poll(const Model& model,
                     const cen::keyboard_event& event,
                     entt::dispatcher& dispatcher)
{
  if (IsEnabled(model)) {
    if (event.pressed() && event.is_active(mKey) && event.is_only_active(mModifiers)) {
      Activate(dispatcher);
    }
  }
}

}  // namespace Tactile

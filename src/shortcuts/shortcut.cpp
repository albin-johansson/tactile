#include "shortcut.hpp"

namespace Tactile {

void AShortcut::Poll(const Model& model,
                     const cen::keyboard_event& event,
                     entt::dispatcher& dispatcher)
{
  if (IsEnabled(model))
  {
    const bool areModsActive =
        mModifiers != cen::key_mod::none ? event.is_only_active(mModifiers) : true;

    if (event.pressed() && event.is_active(mKey) && areModsActive)
    {
      Activate(dispatcher);
    }
  }
}

}  // namespace Tactile

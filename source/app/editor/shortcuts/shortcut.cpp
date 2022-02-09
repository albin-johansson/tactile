#include "shortcut.hpp"

namespace tactile {

void AShortcut::Poll(const document_model& model,
                     const widget_manager& widgets,
                     const cen::keyboard_event& event,
                     entt::dispatcher& dispatcher)
{
  if (IsEnabled(model, widgets)) {
    if (event.pressed() && event.scan() == mKey && event.is_only_active(mModifiers)) {
      Activate(dispatcher);
    }
  }
}

}  // namespace tactile

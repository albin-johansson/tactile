#include "dock_widget.hpp"

#include "editor/gui/scoped.hpp"

namespace tactile {

dock_widget::dock_widget(const char* title, const ImGuiWindowFlags flags)
    : mTitle{title}
    , mFlags{flags}
{}

void dock_widget::update(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  bool visible = is_visible();

  if (!visible) {
    return;
  }

  bool* show = mHasCloseButton ? &visible : nullptr;

  scoped::window dock{mTitle, mFlags, show};
  mHasFocus = dock.has_focus();

  if (dock.is_open()) {
    on_update(registry, dispatcher);
  }

  set_visible(visible);
}

void dock_widget::set_close_button_enabled(const bool enabled)
{
  mHasCloseButton = enabled;
}

}  // namespace tactile
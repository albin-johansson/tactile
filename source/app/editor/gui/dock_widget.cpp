#include "dock_widget.hpp"

#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"
#include "misc/throw.hpp"

namespace tactile {

dock_widget::dock_widget(const char* title, const ImGuiWindowFlags flags)
    : mTitle{title}
    , mWindowFlags{flags}
{
  if (!mTitle) {
    throw_traced(tactile_error{"Invalid null dock widget title!"});
  }
}

void dock_widget::update(const document_model& model, entt::dispatcher& dispatcher)
{
  bool visible = is_visible();

  if (!visible) {
    return;
  }

  bool* show = mHasCloseButton ? &visible : nullptr;

  scoped::window dock{mTitle, mWindowFlags, show};
  mHasFocus = dock.has_focus();

  if (dock.is_open()) {
    on_update(model, dispatcher);
  }

  set_visible(visible);
}

void dock_widget::set_close_button_enabled(const bool enabled)
{
  mHasCloseButton = enabled;
}

void dock_widget::set_focus_flags(const ImGuiFocusedFlags flags)
{
  mFocusFlags = flags;
}

}  // namespace tactile
#include "increase_viewport_zoom_shortcut.hpp"

#include "editor/events/viewport_events.hpp"
#include "editor/model.hpp"

namespace Tactile {

IncreaseViewportZoomShortcut::IncreaseViewportZoomShortcut()
    : AShortcut{cen::scan_code{SDLK_PLUS}, KMOD_LCTRL}
{}

void IncreaseViewportZoomShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<IncreaseZoomEvent>();
}

auto IncreaseViewportZoomShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument();
}

}  // namespace Tactile

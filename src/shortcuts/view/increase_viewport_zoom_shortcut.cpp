#include "increase_viewport_zoom_shortcut.hpp"

#include "core/model.hpp"
#include "events/viewport_events.hpp"

namespace Tactile {

IncreaseViewportZoomShortcut::IncreaseViewportZoomShortcut()
    : AShortcut{cen::scan_code{SDLK_PLUS}, cen::key_mod::left_ctrl}
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

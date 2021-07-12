#include "increase_viewport_zoom_shortcut.hpp"

#include "core/events/viewport/increase_viewport_zoom_event.hpp"
#include "core/model.hpp"

namespace Tactile {

IncreaseViewportZoomShortcut::IncreaseViewportZoomShortcut()
    : AShortcut{cen::scan_code{SDLK_PLUS}, cen::key_mod::left_ctrl}
{}

void IncreaseViewportZoomShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<IncreaseViewportZoomEvent>();
}

auto IncreaseViewportZoomShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.GetActiveDocument() != nullptr;
}

}  // namespace Tactile

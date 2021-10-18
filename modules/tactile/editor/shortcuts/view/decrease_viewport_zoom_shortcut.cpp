#include "decrease_viewport_zoom_shortcut.hpp"

#include "tactile/editor/events/viewport_events.hpp"
#include "tactile/editor/model.hpp"

namespace Tactile {

DecreaseViewportZoomShortcut::DecreaseViewportZoomShortcut()
    : AShortcut{cen::scan_code{SDLK_MINUS}, cen::key_mod::left_ctrl}
{}

void DecreaseViewportZoomShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<DecreaseZoomEvent>();
}

auto DecreaseViewportZoomShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.CanDecreaseViewportTileSize();
}

}  // namespace Tactile

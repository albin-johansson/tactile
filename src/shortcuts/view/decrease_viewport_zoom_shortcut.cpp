#include "decrease_viewport_zoom_shortcut.hpp"

#include "core/model.hpp"
#include "events/viewport/decrease_viewport_zoom_event.hpp"

namespace Tactile {

DecreaseViewportZoomShortcut::DecreaseViewportZoomShortcut()
    : AShortcut{cen::scan_code{SDLK_MINUS}, cen::key_mod::left_ctrl}
{}

void DecreaseViewportZoomShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<DecreaseViewportZoomEvent>();
}

auto DecreaseViewportZoomShortcut::IsEnabled(const Model& model) const -> bool
{
  const auto* document = model.GetActiveDocument();
  return document && document->CanDecreaseViewportTileSize();
}

}  // namespace Tactile

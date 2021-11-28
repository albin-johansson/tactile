#include "center_viewport_shortcut.hpp"

#include "editor/events/viewport_events.hpp"
#include "editor/model.hpp"

namespace Tactile {

CenterViewportShortcut::CenterViewportShortcut()
    : AShortcut{cen::scancodes::space, KMOD_LCTRL}
{}

void CenterViewportShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<CenterViewportEvent>();
}

auto CenterViewportShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument();
}

}  // namespace Tactile

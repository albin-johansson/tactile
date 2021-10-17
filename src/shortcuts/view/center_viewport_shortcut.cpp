#include "center_viewport_shortcut.hpp"

#include "core/model.hpp"
#include "gui/events/viewport_events.hpp"

namespace Tactile {

CenterViewportShortcut::CenterViewportShortcut()
    : AShortcut{cen::scancodes::space, cen::key_mod::left_ctrl}
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

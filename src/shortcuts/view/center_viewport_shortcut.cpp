#include "center_viewport_shortcut.hpp"

#include "core/events/viewport/center_viewport_event.hpp"
#include "core/model.hpp"

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
  return model.GetActiveDocument() != nullptr;
}

}  // namespace Tactile

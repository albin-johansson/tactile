#include "pan_left_shortcut.hpp"

#include "core/model.hpp"
#include "gui/events/viewport_events.hpp"

namespace Tactile {

PanLeftShortcut::PanLeftShortcut()
    : AShortcut{cen::scancodes::left, cen::key_mod::left_ctrl}
{}

void PanLeftShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanLeftEvent>();
}

auto PanLeftShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument();
}

}  // namespace Tactile

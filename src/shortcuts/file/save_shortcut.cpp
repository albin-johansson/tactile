#include "save_shortcut.hpp"

#include "core/model.hpp"
#include "events/save_event.hpp"

namespace Tactile {

SaveShortcut::SaveShortcut() : AShortcut{cen::scancodes::s, cen::key_mod::left_ctrl}
{}

void SaveShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SaveEvent>();
}

auto SaveShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.CanSaveDocument();
}

}  // namespace Tactile

#include "redo_shortcut.hpp"

#include "editor/events/command_events.hpp"
#include "editor/model.hpp"

namespace Tactile {

RedoShortcut::RedoShortcut() : AShortcut{cen::scancodes::y, cen::key_mod::left_ctrl}
{}

void RedoShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RedoEvent>();
}

auto RedoShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.CanRedo();
}

}  // namespace Tactile

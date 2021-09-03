#include "undo_shortcut.hpp"

#include "core/model.hpp"
#include "events/undo_event.hpp"

namespace Tactile {

UndoShortcut::UndoShortcut() : AShortcut{cen::scancodes::z, cen::key_mod::left_ctrl}
{}

void UndoShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<UndoEvent>();
}

auto UndoShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.CanUndo();
}

}  // namespace Tactile

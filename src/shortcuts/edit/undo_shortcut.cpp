#include "undo_shortcut.hpp"

#include "core/events/undo_event.hpp"
#include "core/model.hpp"

namespace Tactile {

UndoShortcut::UndoShortcut()
    : AShortcut{cen::scancodes::z, cen::key_mod::left_ctrl}
{}

void UndoShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<UndoEvent>();
}

auto UndoShortcut::IsEnabled(const Model& model) const -> bool
{
  const auto* document = model.GetActiveDocument();
  return document && document->CanUndo();
}

}  // namespace Tactile

#include "redo_shortcut.hpp"

#include "core/model.hpp"
#include "events/redo_event.hpp"

namespace Tactile {

RedoShortcut::RedoShortcut() : AShortcut{cen::scancodes::y, cen::key_mod::left_ctrl}
{}

void RedoShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RedoEvent>();
}

auto RedoShortcut::IsEnabled(const Model& model) const -> bool
{
  const auto* document = model.GetActiveDocument();
  return document && document->CanRedo();
}

}  // namespace Tactile

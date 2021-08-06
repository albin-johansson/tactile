#include "remove_column_shortcut.hpp"

#include "core/model.hpp"
#include "events/maps/remove_column_event.hpp"

namespace Tactile {

RemoveColumnShortcut::RemoveColumnShortcut()
    : AShortcut{cen::scancodes::c, cen::key_mod::left_shift | cen::key_mod::left_alt}
{}

void RemoveColumnShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RemoveColumnEvent>();
}

auto RemoveColumnShortcut::IsEnabled(const Model& model) const -> bool
{
  const auto* document = model.GetActiveDocument();
  return document && document->GetColumnCount() > 1_col;
}

}  // namespace Tactile

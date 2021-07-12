#include "remove_row_shortcut.hpp"

#include "core/events/maps/remove_row_event.hpp"
#include "core/model.hpp"

namespace Tactile {

RemoveRowShortcut::RemoveRowShortcut()
    : AShortcut{cen::scancodes::r, cen::key_mod::left_alt | cen::key_mod::left_shift}
{}

void RemoveRowShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RemoveRowEvent>();
}

auto RemoveRowShortcut::IsEnabled(const Model& model) const -> bool
{
  const auto* document = model.GetActiveDocument();
  return document && document->GetRowCount() > 1_row;
}

}  // namespace Tactile

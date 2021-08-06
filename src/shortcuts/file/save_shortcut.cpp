#include "save_shortcut.hpp"

#include "events/save_event.hpp"

namespace Tactile {

SaveShortcut::SaveShortcut() : AShortcut{cen::scancodes::s, cen::key_mod::left_ctrl}
{}

void SaveShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SaveEvent>();
}

}  // namespace Tactile

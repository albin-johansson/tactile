#include "save_as_shortcut.hpp"

#include "events/save_as_request_event.hpp"

namespace Tactile {

SaveAsShortcut::SaveAsShortcut()
    : AShortcut{cen::scancodes::s,
                cen::key_mod::left_ctrl | cen::key_mod::left_shift}
{}

void SaveAsShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SaveAsRequestEvent>();
}

}  // namespace Tactile

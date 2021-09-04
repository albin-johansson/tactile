#include "enable_stamp_shortcut.hpp"

#include "events/tool_events.hpp"

namespace Tactile {

EnableStampShortcut::EnableStampShortcut() : AShortcut{cen::scancodes::s}
{}

void EnableStampShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(MouseToolType::Stamp);
}

}  // namespace Tactile

#include "enable_stamp_shortcut.hpp"

#include "events/tools/select_tool_event.hpp"

namespace Tactile {

EnableStampShortcut::EnableStampShortcut() : AShortcut{cen::scancodes::s}
{}

void EnableStampShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(MouseToolType::Stamp);
}

}  // namespace Tactile

#include "enable_eraser_shortcut.hpp"

#include "events/tool_events.hpp"

namespace Tactile {

EnableEraserShortcut::EnableEraserShortcut() : AShortcut{cen::scancodes::e}
{}

void EnableEraserShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(MouseToolType::Eraser);
}

}  // namespace Tactile

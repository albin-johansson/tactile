#include "tool_system.hpp"

#include "bucket_tool_system.hpp"
#include "core/components/tool.hpp"
#include "eraser_tool_system.hpp"
#include "object_selection_tool_system.hpp"
#include "stamp_tool_system.hpp"

namespace tactile::sys {

void select_tool(entt::registry& registry, const tool_type tool)
{
  auto& active = registry.ctx<comp::active_tool>();
  active.tool = (active.tool == tool) ? tool_type::none : tool;
}

void tool_on_pressed(entt::registry& registry,
                     entt::dispatcher& dispatcher,
                     const mouse_info& mouse)
{
  const auto& active = registry.ctx<comp::active_tool>();
  switch (active.tool) {
    case tool_type::none:
      break;

    case tool_type::stamp:
      stamp_tool_on_pressed(registry, mouse);
      break;

    case tool_type::bucket:
      bucket_tool_on_pressed(registry, dispatcher, mouse);
      break;

    case tool_type::eraser:
      eraser_tool_on_pressed(registry, mouse);
      break;

    case tool_type::object_selection:
      object_selection_tool_on_pressed(registry, dispatcher, mouse);
      break;

    case tool_type::rectangle:
    case tool_type::ellipse:
    case tool_type::point:
      // TODO
      break;
  }
}

void tool_on_dragged(entt::registry& registry,
                     [[maybe_unused]] entt::dispatcher& dispatcher,
                     const mouse_info& mouse)
{
  const auto& active = registry.ctx<comp::active_tool>();
  switch (active.tool) {
    case tool_type::none:
      [[fallthrough]];

    case tool_type::bucket:
      break;

    case tool_type::stamp:
      stamp_tool_on_dragged(registry, mouse);
      break;

    case tool_type::eraser:
      eraser_tool_on_dragged(registry, mouse);
      break;

    case tool_type::object_selection:
      object_selection_tool_on_dragged(registry, mouse);
      break;

    case tool_type::rectangle:
    case tool_type::ellipse:
    case tool_type::point:
      // TODO
      break;
  }
}

void tool_on_released(entt::registry& registry,
                      entt::dispatcher& dispatcher,
                      const mouse_info& mouse)
{
  const auto& active = registry.ctx<comp::active_tool>();
  switch (active.tool) {
    case tool_type::none:
      [[fallthrough]];

    case tool_type::bucket:
      break;

    case tool_type::stamp:
      stamp_tool_on_released(registry, dispatcher, mouse);
      break;

    case tool_type::eraser:
      eraser_tool_on_released(registry, dispatcher, mouse);
      break;

    case tool_type::object_selection:
      object_selection_tool_on_released(registry, dispatcher, mouse);
      break;

    case tool_type::rectangle:
    case tool_type::ellipse:
    case tool_type::point:
      // TODO
      break;
  }
}

auto is_tool_enabled(const entt::registry& registry, const tool_type tool) -> bool
{
  const auto& active = registry.ctx<comp::active_tool>();
  return active.tool == tool;
}

}  // namespace tactile::sys

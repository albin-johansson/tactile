#include "tool_system.hpp"

#include "bucket_tool_system.hpp"
#include "core/components/tool.hpp"
#include "eraser_tool_system.hpp"
#include "object_selection_tool_system.hpp"
#include "stamp_tool_system.hpp"

namespace tactile::sys {

void SelectTool(entt::registry& registry, const tool_type tool)
{
  auto& active = registry.ctx<comp::active_tool>();
  active.tool = (active.tool == tool) ? tool_type::none : tool;
}

void ToolOnPressed(entt::registry& registry,
                   entt::dispatcher& dispatcher,
                   const MouseInfo& mouse)
{
  const auto& active = registry.ctx<comp::active_tool>();
  switch (active.tool) {
    case tool_type::none:
      break;

    case tool_type::stamp:
      StampToolOnPressed(registry, mouse);
      break;

    case tool_type::bucket:
      BucketToolOnPressed(registry, dispatcher, mouse);
      break;

    case tool_type::eraser:
      EraserToolOnPressed(registry, mouse);
      break;

    case tool_type::object_selection:
      ObjectSelectionToolOnPressed(registry, dispatcher, mouse);
      break;
  }
}

void ToolOnDragged(entt::registry& registry,
                   [[maybe_unused]] entt::dispatcher& dispatcher,
                   const MouseInfo& mouse)
{
  const auto& active = registry.ctx<comp::active_tool>();
  switch (active.tool) {
    case tool_type::none:
      [[fallthrough]];

    case tool_type::bucket:
      break;

    case tool_type::stamp:
      StampToolOnDragged(registry, mouse);
      break;

    case tool_type::eraser:
      EraserToolOnDragged(registry, mouse);
      break;

    case tool_type::object_selection:
      ObjectSelectionToolOnDragged(registry, mouse);
      break;
  }
}

void ToolOnReleased(entt::registry& registry,
                    entt::dispatcher& dispatcher,
                    const MouseInfo& mouse)
{
  const auto& active = registry.ctx<comp::active_tool>();
  switch (active.tool) {
    case tool_type::none:
      [[fallthrough]];

    case tool_type::bucket:
      break;

    case tool_type::stamp:
      StampToolOnReleased(registry, dispatcher, mouse);
      break;

    case tool_type::eraser:
      EraserToolOnReleased(registry, dispatcher, mouse);
      break;

    case tool_type::object_selection:
      ObjectSelectionToolOnReleased(registry, dispatcher, mouse);
      break;
  }
}

auto IsStampEnabled(const entt::registry& registry) -> bool
{
  const auto& active = registry.ctx<comp::active_tool>();
  return active.tool == tool_type::stamp;
}

auto IsEraserEnabled(const entt::registry& registry) -> bool
{
  const auto& active = registry.ctx<comp::active_tool>();
  return active.tool == tool_type::eraser;
}

auto IsBucketEnabled(const entt::registry& registry) -> bool
{
  const auto& active = registry.ctx<comp::active_tool>();
  return active.tool == tool_type::bucket;
}

auto IsObjectSelectionEnabled(const entt::registry& registry) -> bool
{
  const auto& active = registry.ctx<comp::active_tool>();
  return active.tool == tool_type::object_selection;
}

}  // namespace tactile::sys

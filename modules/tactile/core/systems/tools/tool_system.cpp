#include "tool_system.hpp"

#include "bucket_tool_system.hpp"
#include "core/components/tool.hpp"
#include "eraser_tool_system.hpp"
#include "object_selection_tool_system.hpp"
#include "stamp_tool_system.hpp"

namespace tactile::Sys {

void SelectTool(entt::registry& registry, const ToolType tool)
{
  auto& active = registry.ctx<ActiveTool>();
  active.tool = (active.tool == tool) ? ToolType::None : tool;
}

void ToolOnPressed(entt::registry& registry,
                   entt::dispatcher& dispatcher,
                   const MouseInfo& mouse)
{
  const auto& active = registry.ctx<ActiveTool>();
  switch (active.tool) {
    case ToolType::None:
      break;

    case ToolType::Stamp:
      StampToolOnPressed(registry, mouse);
      break;

    case ToolType::Bucket:
      BucketToolOnPressed(registry, dispatcher, mouse);
      break;

    case ToolType::Eraser:
      EraserToolOnPressed(registry, mouse);
      break;

    case ToolType::ObjectSelection:
      ObjectSelectionToolOnPressed(registry, dispatcher, mouse);
      break;
  }
}

void ToolOnDragged(entt::registry& registry,
                   [[maybe_unused]] entt::dispatcher& dispatcher,
                   const MouseInfo& mouse)
{
  const auto& active = registry.ctx<ActiveTool>();
  switch (active.tool) {
    case ToolType::None:
      [[fallthrough]];

    case ToolType::Bucket:
      break;

    case ToolType::Stamp:
      StampToolOnDragged(registry, mouse);
      break;

    case ToolType::Eraser:
      EraserToolOnDragged(registry, mouse);
      break;

    case ToolType::ObjectSelection:
      ObjectSelectionToolOnDragged(registry, mouse);
      break;
  }
}

void ToolOnReleased(entt::registry& registry,
                    entt::dispatcher& dispatcher,
                    const MouseInfo& mouse)
{
  const auto& active = registry.ctx<ActiveTool>();
  switch (active.tool) {
    case ToolType::None:
      [[fallthrough]];

    case ToolType::Bucket:
      break;

    case ToolType::Stamp:
      StampToolOnReleased(registry, dispatcher, mouse);
      break;

    case ToolType::Eraser:
      EraserToolOnReleased(registry, dispatcher, mouse);
      break;

    case ToolType::ObjectSelection:
      ObjectSelectionToolOnReleased(registry, dispatcher, mouse);
      break;
  }
}

auto IsStampEnabled(const entt::registry& registry) -> bool
{
  const auto& active = registry.ctx<ActiveTool>();
  return active.tool == ToolType::Stamp;
}

auto IsEraserEnabled(const entt::registry& registry) -> bool
{
  const auto& active = registry.ctx<ActiveTool>();
  return active.tool == ToolType::Eraser;
}

auto IsBucketEnabled(const entt::registry& registry) -> bool
{
  const auto& active = registry.ctx<ActiveTool>();
  return active.tool == ToolType::Bucket;
}

auto IsObjectSelectionEnabled(const entt::registry& registry) -> bool
{
  const auto& active = registry.ctx<ActiveTool>();
  return active.tool == ToolType::ObjectSelection;
}

}  // namespace tactile::Sys

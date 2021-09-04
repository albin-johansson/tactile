#include "tool_system.hpp"

#include "core/components/tool.hpp"
#include "stamp_tool_system.hpp"

namespace Tactile::Sys {

void SelectTool(entt::registry& registry, const MouseToolType tool)
{
  auto& active = registry.ctx<ActiveTool>();
  active.tool = tool;
}

void ToolOnPressed(entt::registry& registry, const MouseInfo& mouse)
{
  const auto& active = registry.ctx<ActiveTool>();
  switch (active.tool)
  {
    case MouseToolType::None:
      break;

    case MouseToolType::Stamp:
      StampToolOnPressed(registry, mouse);
      break;

    case MouseToolType::Bucket:
      break;

    case MouseToolType::Eraser:
      break;
  }
}

void ToolOnDragged(entt::registry& registry, const MouseInfo& mouse)
{
  const auto& active = registry.ctx<ActiveTool>();
  switch (active.tool)
  {
    case MouseToolType::None:
      break;

    case MouseToolType::Stamp:
      StampToolOnDragged(registry, mouse);
      break;

    case MouseToolType::Bucket:
      break;

    case MouseToolType::Eraser:
      break;
  }
}

void ToolOnReleased(entt::registry& registry,
                    entt::dispatcher& dispatcher,
                    const MouseInfo& mouse)
{
  const auto& active = registry.ctx<ActiveTool>();
  switch (active.tool)
  {
    case MouseToolType::None:
      break;

    case MouseToolType::Stamp:
      StampToolOnReleased(registry, dispatcher, mouse);
      break;

    case MouseToolType::Bucket:
      break;

    case MouseToolType::Eraser:
      break;
  }
}

auto IsStampEnabled(const entt::registry& registry) -> bool
{
  const auto& active = registry.ctx<ActiveTool>();
  return active.tool == MouseToolType::Stamp;
}

auto IsEraserEnabled(const entt::registry& registry) -> bool
{
  const auto& active = registry.ctx<ActiveTool>();
  return active.tool == MouseToolType::Eraser;
}

auto IsBucketEnabled(const entt::registry& registry) -> bool
{
  const auto& active = registry.ctx<ActiveTool>();
  return active.tool == MouseToolType::Bucket;
}

}  // namespace Tactile::Sys

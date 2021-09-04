#include "tool_system.hpp"

#include "core/components/tool.hpp"

namespace Tactile::Sys {

void SelectTool(entt::registry& registry, const MouseToolType tool)
{
  auto& active = registry.ctx<ActiveTool>();
  active.tool = tool;
}

void ToolOnPressed(entt::registry& registry, const MouseInfo& mouse)
{
}

void ToolOnDragged(entt::registry& registry, const MouseInfo& mouse)
{
}

void ToolOnReleased(entt::registry& registry, const MouseInfo& mouse)
{
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

#include "enable_bucket_tool.hpp"

#include "core/model.hpp"
#include "events/tool_events.hpp"

namespace Tactile {

EnableBucketShortcut::EnableBucketShortcut() : AShortcut{cen::scancodes::b}
{}

void EnableBucketShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(MouseToolType::Bucket);
}

auto EnableBucketShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument();
}

}  // namespace Tactile

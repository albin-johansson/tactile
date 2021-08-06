#include "enable_bucket_tool.hpp"

#include "events/tools/select_tool_event.hpp"

namespace Tactile {

EnableBucketShortcut::EnableBucketShortcut() : AShortcut{cen::scancodes::b}
{}

void EnableBucketShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(MouseToolType::Bucket);
}

}  // namespace Tactile

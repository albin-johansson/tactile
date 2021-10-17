#include "enable_bucket_tool.hpp"

#include "editor/events/tool_events.hpp"
#include "editor/gui/widgets/focus.hpp"
#include "editor/model.hpp"

namespace Tactile {

EnableBucketShortcut::EnableBucketShortcut() : AShortcut{cen::scancodes::b}
{}

void EnableBucketShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(ToolType::Bucket);
}

auto EnableBucketShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument() && IsEditorFocused();
}

}  // namespace Tactile

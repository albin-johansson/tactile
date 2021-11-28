#include "edit_shortcuts.hpp"

#include "core/map.hpp"
#include "editor/events/command_events.hpp"
#include "editor/events/map_events.hpp"
#include "editor/events/tool_events.hpp"
#include "editor/gui/widgets/focus.hpp"
#include "editor/gui/widgets/menus/map_menu.hpp"
#include "editor/model.hpp"

namespace Tactile {

UndoShortcut::UndoShortcut() : AShortcut{cen::scancodes::z, KMOD_LCTRL}
{}

void UndoShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<UndoEvent>();
}

auto UndoShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.CanUndo();
}

RedoShortcut::RedoShortcut() : AShortcut{cen::scancodes::y, KMOD_LCTRL}
{}

void RedoShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RedoEvent>();
}

auto RedoShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.CanRedo();
}

AddRowShortcut::AddRowShortcut() : AShortcut{cen::scancodes::r, KMOD_LALT}
{}

void AddRowShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<AddRowEvent>();
}

auto AddRowShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument();
}

AddColumnShortcut::AddColumnShortcut() : AShortcut{cen::scancodes::c, KMOD_LALT}
{}

void AddColumnShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<AddColumnEvent>();
}

auto AddColumnShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument();
}

RemoveRowShortcut::RemoveRowShortcut()
    : AShortcut{cen::scancodes::r, KMOD_LALT | KMOD_LSHIFT}
{}

void RemoveRowShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RemoveRowEvent>();
}

auto RemoveRowShortcut::IsEnabled(const Model& model) const -> bool
{
  if (const auto* registry = model.GetActiveRegistry()) {
    const auto& map = registry->ctx<Map>();
    return map.row_count > 1;
  }
  else {
    return false;
  }
}

RemoveColumnShortcut::RemoveColumnShortcut()
    : AShortcut{cen::scancodes::c, KMOD_LSHIFT | KMOD_LALT}
{}

void RemoveColumnShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RemoveColumnEvent>();
}

auto RemoveColumnShortcut::IsEnabled(const Model& model) const -> bool
{
  if (const auto* registry = model.GetActiveRegistry()) {
    const auto& map = registry->ctx<Map>();
    return map.column_count > 1;
  }
  else {
    return false;
  }
}

AddTilesetShortcut::AddTilesetShortcut() : AShortcut{cen::scancodes::t, KMOD_LCTRL}
{}

void AddTilesetShortcut::Activate(entt::dispatcher&)
{
  ShowTilesetDialog();
}

auto AddTilesetShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument();
}

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

EnableEraserShortcut::EnableEraserShortcut() : AShortcut{cen::scancodes::e}
{}

void EnableEraserShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(ToolType::Eraser);
}

auto EnableEraserShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument() && IsEditorFocused();
}

EnableStampShortcut::EnableStampShortcut() : AShortcut{cen::scancodes::s}
{}

void EnableStampShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(ToolType::Stamp);
}

auto EnableStampShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument() && IsEditorFocused();
}

}  // namespace Tactile

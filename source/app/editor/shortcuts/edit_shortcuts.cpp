#include "edit_shortcuts.hpp"

#include "core/map.hpp"
#include "editor/events/command_events.hpp"
#include "editor/events/map_events.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/events/tool_events.hpp"
#include "editor/gui/menus/map_menu.hpp"
#include "editor/gui/widget_manager.hpp"
#include "editor/model.hpp"
#include "mappings.hpp"

namespace tactile {

UndoShortcut::UndoShortcut() : AShortcut{cen::scancodes::z, gPrimaryModifier} {}

void UndoShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<undo_event>();
}

auto UndoShortcut::IsEnabled(const Model& model, const WidgetManager&) const -> bool
{
  return model.CanUndo();
}

RedoShortcut::RedoShortcut() : AShortcut{cen::scancodes::y, gPrimaryModifier} {}

void RedoShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<redo_event>();
}

auto RedoShortcut::IsEnabled(const Model& model, const WidgetManager&) const -> bool
{
  return model.CanRedo();
}

AddRowShortcut::AddRowShortcut() : AShortcut{cen::scancodes::r, gSecondaryModifier} {}

void AddRowShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<add_row_event>();
}

auto AddRowShortcut::IsEnabled(const Model& model, const WidgetManager&) const -> bool
{
  return model.HasActiveDocument();
}

AddColumnShortcut::AddColumnShortcut() : AShortcut{cen::scancodes::c, gSecondaryModifier}
{}

void AddColumnShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<add_column_event>();
}

auto AddColumnShortcut::IsEnabled(const Model& model, const WidgetManager&) const -> bool
{
  return model.HasActiveDocument();
}

RemoveRowShortcut::RemoveRowShortcut()
    : AShortcut{cen::scancodes::r, gSecondaryModifier | cen::key_mod::lshift}
{}

void RemoveRowShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<remove_row_event>();
}

auto RemoveRowShortcut::IsEnabled(const Model& model, const WidgetManager&) const -> bool
{
  if (const auto* registry = model.GetActiveRegistry()) {
    const auto& map = registry->ctx<MapInfo>();
    return map.row_count > 1;
  }
  else {
    return false;
  }
}

RemoveColumnShortcut::RemoveColumnShortcut()
    : AShortcut{cen::scancodes::c, gSecondaryModifier | cen::key_mod::lshift}
{}

void RemoveColumnShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<remove_column_event>();
}

auto RemoveColumnShortcut::IsEnabled(const Model& model, const WidgetManager&) const
    -> bool
{
  if (const auto* registry = model.GetActiveRegistry()) {
    const auto& map = registry->ctx<MapInfo>();
    return map.column_count > 1;
  }
  else {
    return false;
  }
}

AddTilesetShortcut::AddTilesetShortcut() : AShortcut{cen::scancodes::t, gPrimaryModifier}
{}

void AddTilesetShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<show_add_tileset_dialog_event>();
}

auto AddTilesetShortcut::IsEnabled(const Model& model, const WidgetManager&) const -> bool
{
  return model.HasActiveDocument();
}

EnableBucketShortcut::EnableBucketShortcut() : AShortcut{cen::scancodes::b} {}

void EnableBucketShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<select_tool_event>(tool_type::bucket);
}

auto EnableBucketShortcut::IsEnabled(const Model& model,
                                     const WidgetManager& widgets) const -> bool
{
  return model.IsBucketPossible() && widgets.IsEditorFocused();
}

EnableEraserShortcut::EnableEraserShortcut() : AShortcut{cen::scancodes::e} {}

void EnableEraserShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<select_tool_event>(tool_type::eraser);
}

auto EnableEraserShortcut::IsEnabled(const Model& model,
                                     const WidgetManager& widgets) const -> bool
{
  return model.IsEraserPossible() && widgets.IsEditorFocused();
}

EnableStampShortcut::EnableStampShortcut() : AShortcut{cen::scancodes::s} {}

void EnableStampShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<select_tool_event>(tool_type::stamp);
}

auto EnableStampShortcut::IsEnabled(const Model& model,
                                    const WidgetManager& widgets) const -> bool
{
  return model.IsStampPossible() && widgets.IsEditorFocused();
}

EnableObjectSelectionShortcut::EnableObjectSelectionShortcut()
    : AShortcut{cen::scancodes::q}
{}

void EnableObjectSelectionShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<select_tool_event>(tool_type::object_selection);
}

auto EnableObjectSelectionShortcut::IsEnabled(const Model& model,
                                              const WidgetManager& widgets) const -> bool
{
  return model.IsObjectSelectionPossible() && widgets.IsEditorFocused();
}

}  // namespace tactile

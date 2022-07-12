/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "property_dock.hpp"

#include <utility>  // move

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/common/maybe.hpp"
#include "core/context/context_visitor.hpp"
#include "core/document/tileset_document.hpp"
#include "core/event/document_events.hpp"
#include "core/event/layer_events.hpp"
#include "core/event/object_events.hpp"
#include "core/event/property_events.hpp"
#include "core/event/tileset_events.hpp"
#include "core/layer/group_layer.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/map.hpp"
#include "core/model.hpp"
#include "core/tileset/tileset.hpp"
#include "editor/ui/common/filename_filter.hpp"
#include "editor/ui/common/input_widgets.hpp"
#include "editor/ui/icons.hpp"
#include "editor/ui/properties/items/property_item_context_menu.hpp"
#include "editor/ui/scoped.hpp"
#include "editor/ui/shared/dialog_state.hpp"
#include "editor/ui/shared/dialogs.hpp"
#include "io/persistence/preferences.hpp"

using namespace tactile;

namespace tactile::ui {
namespace {

inline PropertyItemContextMenuState _context_state;
inline Maybe<std::string>           _rename_target;
inline Maybe<std::string>           _change_type_target;
constinit bool                      _is_focused = false;

void _prepare_table_row(const char* label)
{
  ImGui::TableNextRow();
  ImGui::TableNextColumn();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(label);
}

[[nodiscard]] auto _native_name_row(const std::string& name,
                                    const bool         validateAsFileName = false)
    -> Maybe<std::string>
{
  _prepare_table_row("Name");

  ImGui::TableNextColumn();

  auto flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;
  if (validateAsFileName) {
    flags |= ImGuiInputTextFlags_CallbackCharFilter;
    return input_string("##NativeNameRowInput", name, nullptr, flags, filename_filter);
  }
  else {
    return input_string("##NativeNameRowInput", name, nullptr, flags);
  }
}

void _native_read_only_row(const char* label, const char* value)
{
  _prepare_table_row(label);

  ImGui::TableNextColumn();
  ImGui::TextUnformatted(value);
}

void _native_read_only_row(const char* label, const float value)
{
  _prepare_table_row(label);

  ImGui::TableNextColumn();
  ImGui::Text("%.2f", value);
}

void _native_read_only_row(const char* label, const int32 value)
{
  _prepare_table_row(label);

  ImGui::TableNextColumn();
  ImGui::Text("%d", value);
}

void _native_read_only_row(const char* label, const usize value)
{
  _prepare_table_row(label);

  ImGui::TableNextColumn();
  ImGui::Text("%llu", static_cast<ulonglong>(value)); /* Cast to avoid format warnings */
}

void _show_native_map_properties(const Map& map)
{
  _native_read_only_row("Type", "Map");
  _native_read_only_row("Name", map.get_name().c_str());

  _native_read_only_row("Tile width", map.tile_size().x);
  _native_read_only_row("Tile height", map.tile_size().y);

  _native_read_only_row("Row count", map.row_count());
  _native_read_only_row("Column count", map.column_count());
}

void _show_native_tileset_properties(const Tileset& tileset, entt::dispatcher& dispatcher)
{
  _native_read_only_row("Type", "Tileset");

  if (const auto updatedName = _native_name_row(tileset.get_name().c_str(), true);
      updatedName && !updatedName->empty()) {
    dispatcher.enqueue<RenameTilesetEvent>(tileset.get_uuid(), *updatedName);
  }

  _native_read_only_row("Tile count", tileset.tile_count());
  _native_read_only_row("Column count", tileset.column_count());

  _native_read_only_row("Tile width", tileset.tile_size().x);
  _native_read_only_row("Tile height", tileset.tile_size().y);
}

// TODO think about how to show the ref info? Maybe as overlay in tileset view?
void _show_native_tileset_ref_properties(const TilesetRef& ref)
{
  const auto& tileset = ref.view_tileset();

  _native_read_only_row("Type", "Tileset");
  _native_read_only_row("Name", tileset.get_name().c_str());

  _native_read_only_row("Tile count", tileset.tile_count());
  _native_read_only_row("Column count", tileset.column_count());

  _native_read_only_row("Tile width", tileset.tile_size().x);
  _native_read_only_row("Tile height", tileset.tile_size().y);

  _native_read_only_row("First Tile ID", ref.first_tile());
  _native_read_only_row("Last Tile ID", ref.last_tile());

  _native_read_only_row("Embedded", ref.is_embedded());
}

void _show_native_layer_properties(const ILayer& layer, entt::dispatcher& dispatcher)
{
  switch (layer.get_type()) {
    case LayerType::TileLayer:
      _native_read_only_row("Type", "Tile Layer");
      break;

    case LayerType::ObjectLayer:
      _native_read_only_row("Type", "Object Layer");
      break;

    case LayerType::GroupLayer:
      _native_read_only_row("Type", "Group Layer");
      break;
  }

  _prepare_table_row("Opacity");
  ImGui::TableNextColumn();
  if (const auto value =
          input_float("##_native_opacity_row", layer.get_opacity(), 0.0f, 1.0f)) {
    dispatcher.enqueue<SetLayerOpacityEvent>(layer.get_uuid(), *value);
  }

  _prepare_table_row("Visible");
  ImGui::TableNextColumn();
  if (const auto value = input_bool("##_native_visibility_row", layer.is_visible())) {
    dispatcher.enqueue<SetLayerVisibleEvent>(layer.get_uuid(), *value);
  }
}

void _show_native_object_properties(const Object& object, entt::dispatcher& dispatcher)
{
  switch (object.get_type()) {
    case ObjectType::Rect:
      _native_read_only_row("Type", "Rectangle");
      break;

    case ObjectType::Point:
      _native_read_only_row("Type", "Point");
      break;

    case ObjectType::Ellipse:
      _native_read_only_row("Type", "Ellipse");
      break;
  }

  if (const auto updatedName = _native_name_row(object.get_name())) {
    dispatcher.enqueue<SetObjectNameEvent>(object.get_uuid(), *updatedName);
  }

  const auto& pos = object.get_pos();
  _native_read_only_row("X", pos.x);
  _native_read_only_row("Y", pos.y);

  if (object.get_type() != ObjectType::Point) {
    const auto& size = object.get_size();
    _native_read_only_row("Width", size.x);
    _native_read_only_row("Height", size.y);
  }

  _prepare_table_row("Visible");
  ImGui::TableNextColumn();
  if (const auto visible = input_bool("##ObjectVisible", object.is_visible())) {
    dispatcher.enqueue<SetObjectVisibleEvent>(object.get_uuid(), *visible);
  }

  _prepare_table_row("Tag");

  ImGui::TableNextColumn();
  if (const auto tag = input_string("##NativeObjectTagInput", object.get_tag())) {
    dispatcher.enqueue<SetObjectTagEvent>(object.get_uuid(), *tag);
  }
}

void _show_custom_properties(const IContext&   context,
                             entt::dispatcher& dispatcher,
                             bool&             isItemContextOpen)
{
  bool first = true;

  for (const auto& [name, value] : context.get_props()) {
    const Scope scope {name.c_str()};

    ImGui::TableNextRow();
    ImGui::TableNextColumn();

    if (first) {
      ImGui::Separator();
    }

    ImGui::AlignTextToFramePadding();
    ImGui::Selectable(name.c_str());

    if (!isItemContextOpen) {
      isItemContextOpen = property_item_context_menu(context.get_uuid(),
                                                     dispatcher,
                                                     name,
                                                     _context_state);
    }

    if (_context_state.show_rename_dialog && !_rename_target) {
      _rename_target = name;
    }

    if (_context_state.show_change_type_dialog && !_change_type_target) {
      _change_type_target = name;
    }

    ImGui::TableNextColumn();

    if (first) {
      ImGui::Separator();
    }

    if (auto updated = input_attribute("##CustomPropertyInput", value)) {
      dispatcher.enqueue<UpdatePropertyEvent>(context.get_uuid(),
                                              name,
                                              std::move(*updated));
    }

    first = false;
  }
}

void _update_conditional_tileset_button(const ADocument&  document,
                                        entt::dispatcher& dispatcher)
{
  if (document.get_type() == DocumentType::Map) {
    // TODO if active context is tileset, show extra stuff

    /*if (centered_button("Open Tileset")) {
      dispatcher.enqueue<OpenDocumentEvent>(ref.source_tileset);
      dispatcher.enqueue<SelectDocumentEvent>(ref.source_tileset);
    }*/
  }
}

struct ContextPropertyVisitor final : IContextVisitor
{
  entt::dispatcher* dispatcher {};

  void visit(const Map& map) override { _show_native_map_properties(map); }

  void visit(const TileLayer& layer) override
  {
    _show_native_layer_properties(layer, *dispatcher);
  }

  void visit(const ObjectLayer& layer) override
  {
    _show_native_layer_properties(layer, *dispatcher);
  }

  void visit(const GroupLayer& layer) override
  {
    _show_native_layer_properties(layer, *dispatcher);
  }

  void visit(const Object& object) override
  {
    _show_native_object_properties(object, *dispatcher);
  }

  void visit(const Tileset& tileset) override
  {
    _show_native_tileset_properties(tileset, *dispatcher);
  }

  void visit(const Tile&) override
  {
    // TODO
  }
};

void _update_property_table(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  constexpr auto flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
                         ImGuiTableFlags_ScrollY | ImGuiTableFlags_PadOuterX;

  const auto& document = model.require_active_document();
  const auto& contextId = document.active_context_id();
  const auto& context = document.view_context(contextId);

  _update_conditional_tileset_button(document, dispatcher);

  if (Table table {"##PropertyTable", 2, flags}; table.is_open()) {
    ContextPropertyVisitor visitor;
    visitor.dispatcher = &dispatcher;
    context.accept(visitor);

    bool isItemContextOpen = false;
    _show_custom_properties(context, dispatcher, isItemContextOpen);

    if (!isItemContextOpen) {
      if (auto popup = Popup::for_window("##PropertyTableContext"); popup.is_open()) {
        _context_state.show_add_dialog =
            ImGui::MenuItem(TAC_ICON_ADD " Add New Property...");
      }
    }
  }

  if (_context_state.show_add_dialog) {
    dispatcher.enqueue<ShowAddPropertyDialogEvent>();
    _context_state.show_add_dialog = false;
  }

  if (_context_state.show_rename_dialog) {
    dispatcher.enqueue<ShowRenamePropertyDialogEvent>(_rename_target.value());
    _rename_target.reset();
    _context_state.show_rename_dialog = false;
  }

  if (_context_state.show_change_type_dialog) {
    const auto& targetName = _change_type_target.value();
    const auto  type = context.get_props().at(targetName).type();
    dispatcher.enqueue<ShowChangePropertyTypeDialogEvent>(targetName, type);
    _change_type_target.reset();
    _context_state.show_change_type_dialog = false;
  }
}

}  // namespace

void update_property_dock(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  auto& prefs = io::get_preferences();

  if (!prefs.show_property_dock) {
    return;
  }

  constexpr auto flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

  Window window {"Properties", flags, &prefs.show_property_dock};
  _is_focused = window.has_focus();

  if (window.is_open()) {
    _update_property_table(model, dispatcher);

    auto& dialogs = get_dialogs();
    dialogs.add_property.update(model, dispatcher);
    dialogs.rename_property.update(model, dispatcher);
    dialogs.change_property_type.update(model, dispatcher);
  }
}

auto is_property_dock_focused() -> bool
{
  return _is_focused;
}

}  // namespace tactile::ui

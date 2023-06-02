/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "common/type/maybe.hpp"
#include "common/type/uuid.hpp"
#include "common/util/lookup.hpp"
#include "components/context.hpp"
#include "components/document.hpp"
#include "components/layer.hpp"
#include "components/map.hpp"
#include "components/object.hpp"
#include "components/tile.hpp"
#include "components/tile_format.hpp"
#include "components/tileset.hpp"
#include "io/compression.hpp"
#include "model/event/file_events.hpp"
#include "model/event/layer_events.hpp"
#include "model/event/map_events.hpp"
#include "model/event/object_events.hpp"
#include "model/event/property_events.hpp"
#include "model/event/setting_events.hpp"
#include "model/event/tileset_events.hpp"
#include "model/model.hpp"
#include "model/systems/document_system.hpp"
#include "model/systems/language_system.hpp"
#include "ui/dock/property/dialogs/add_property_dialog.hpp"
#include "ui/dock/property/dialogs/change_property_type_dialog.hpp"
#include "ui/dock/property/dialogs/rename_property_dialog.hpp"
#include "ui/filename_filter.hpp"
#include "ui/widget/attribute_widgets.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {
namespace {

struct PropertyItemContextMenuState final {
  bool show_add_dialog         : 1 {};
  bool show_rename_dialog      : 1 {};
  bool show_change_type_dialog : 1 {};
};

struct PropertyDockState final {
  Maybe<String> rename_target;
  Maybe<String> change_type_target;
  PropertyItemContextMenuState context_state;
  bool has_focus : 1 {};
};

inline PropertyDockState gDockState;

[[nodiscard]] auto _push_property_item_context_menu(const Strings& strings,
                                                    const Entity context_entity,
                                                    const String& property_name,
                                                    PropertyItemContextMenuState& state,
                                                    Dispatcher& dispatcher) -> bool
{
  if (auto popup = Popup::for_item("##PropertyItemPopup"); popup.is_open()) {
    state.show_rename_dialog = ImGui::MenuItem(strings.action.rename_property.c_str());
    state.show_change_type_dialog =
        ImGui::MenuItem(strings.action.change_property_type.c_str());

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.remove_property.c_str())) {
      dispatcher.enqueue<RemovePropertyEvent>(context_entity, property_name);
    }

    return true;
  }

  return false;
}

void _prepare_table_row(const char* label)
{
  ImGui::TableNextRow();
  ImGui::TableNextColumn();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(label);
}

[[nodiscard]] auto _push_native_name_row(const Strings& strings,
                                         const String& name,
                                         const bool validate_as_file_name = false)
    -> Maybe<String>
{
  _prepare_table_row(strings.misc.name.c_str());

  ImGui::TableNextColumn();

  auto flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;
  if (validate_as_file_name) {
    flags |= ImGuiInputTextFlags_CallbackCharFilter;
    return push_string_input(strings,
                             "##NativeNameRowInput",
                             name,
                             nullptr,
                             flags,
                             filename_filter);
  }
  else {
    return push_string_input(strings, "##NativeNameRowInput", name, nullptr, flags);
  }
}

void _push_native_read_only_row(const char* label, const char* value)
{
  _prepare_table_row(label);

  ImGui::TableNextColumn();
  ImGui::TextUnformatted(value);
}

void _push_native_read_only_row(const char* label, const float value)
{
  _prepare_table_row(label);

  ImGui::TableNextColumn();
  ImGui::Text("%.2f", value);
}

void _push_native_read_only_row(const char* label, const int32 value)
{
  _prepare_table_row(label);

  ImGui::TableNextColumn();
  ImGui::Text("%d", value);
}

void _push_native_read_only_row(const char* label, const usize value)
{
  _prepare_table_row(label);

  ImGui::TableNextColumn();
  ImGui::Text("%llu", static_cast<ulonglong>(value));  // Cast to avoid format warnings
}

void _push_native_read_only_row(const char* label, const bool value)
{
  _prepare_table_row(label);

  ImGui::TableNextColumn();

  const Disable disable;
  bool v = value;
  ImGui::Checkbox("##Bool", &v);
}

void _push_native_map_properties(const Model& model,
                                 const Strings& strings,
                                 const Entity map_entity,
                                 Dispatcher& dispatcher)
{
  const auto& context = model.get<Context>(map_entity);
  const auto& map = model.get<Map>(map_entity);
  const auto& tile_format = model.get<TileFormat>(map_entity);

  _push_native_read_only_row(strings.misc.type.c_str(), strings.misc.map.c_str());
  _push_native_read_only_row(strings.misc.name.c_str(), context.name.c_str());

  _push_native_read_only_row(strings.misc.tile_width.c_str(), map.tile_size.x);
  _push_native_read_only_row(strings.misc.tile_height.c_str(), map.tile_size.y);

  _push_native_read_only_row(strings.misc.row_count.c_str(), map.extent.rows);
  _push_native_read_only_row(strings.misc.column_count.c_str(), map.extent.cols);

  _prepare_table_row(strings.misc.tile_encoding.c_str());
  ImGui::TableNextColumn();

  const char* encoding = (tile_format.encoding == TileEncoding::Plain)
                             ? strings.misc.plain_encoding.c_str()
                             : "Base64";
  if (const Combo combo {"##TileEncoding", encoding}; combo.is_open()) {
    if (Selectable::property(strings.misc.plain_encoding.c_str())) {
      dispatcher.enqueue<SetTileFormatEncodingEvent>(TileEncoding::Plain);
    }

    if (Selectable::property("Base64")) {
      dispatcher.enqueue<SetTileFormatEncodingEvent>(TileEncoding::Base64);
    }
  }

  _prepare_table_row(strings.misc.tile_compression.c_str());
  ImGui::TableNextColumn();

  {
    const Disable disable_if {!tile_format.supports_compression()};

    auto compression = strings.misc.none;
    if (tile_format.compression == TileCompression::Zlib) {
      compression = "Zlib";
    }
    else if (tile_format.compression == TileCompression::Zstd) {
      compression = "Zstd";
    }

    if (const Combo combo {"##TileCompression", compression.c_str()}; combo.is_open()) {
      if (Selectable::property(strings.misc.none.c_str())) {
        dispatcher.enqueue<SetTileFormatCompressionEvent>(TileCompression::None);
      }

      if (Selectable::property("Zlib")) {
        dispatcher.enqueue<SetTileFormatCompressionEvent>(TileCompression::Zlib);
      }

      if (Selectable::property("Zstd")) {
        dispatcher.enqueue<SetTileFormatCompressionEvent>(TileCompression::Zstd);
      }
    }

    if (tile_format.compression == TileCompression::Zlib) {
      _prepare_table_row(strings.misc.compression_level.c_str());
      ImGui::TableNextColumn();

      auto level = tile_format.zlib_compression_level;
      if (ImGui::SliderInt("##CompressionLevel",
                           &level,
                           min_zlib_compression_level(),
                           max_zlib_compression_level())) {
        dispatcher.enqueue<SetZlibCompressionLevelEvent>(level);
      }
    }
    else if (tile_format.compression == TileCompression::Zstd) {
      _prepare_table_row(strings.misc.compression_level.c_str());
      ImGui::TableNextColumn();

      auto level = tile_format.zstd_compression_level;
      if (ImGui::SliderInt("##CompressionLevel",
                           &level,
                           min_zstd_compression_level(),
                           max_zstd_compression_level())) {
        dispatcher.enqueue<SetZstdCompressionLevelEvent>(level);
      }
    }
  }
}

void _push_native_tileset_properties(const Model& model,
                                     const Strings& strings,
                                     const Entity tileset_entity,
                                     Dispatcher& dispatcher)
{
  const auto& context = model.get<Context>(tileset_entity);
  const auto& tileset = model.get<Tileset>(tileset_entity);

  _push_native_read_only_row(strings.misc.type.c_str(), strings.misc.tileset.c_str());

  if (const auto updated_name = _push_native_name_row(strings, context.name, true);
      updated_name && !updated_name->empty()) {
    dispatcher.enqueue<RenameTilesetEvent>(tileset_entity, *updated_name);
  }

  _push_native_read_only_row(strings.misc.tile_count.c_str(), tileset.tile_count());
  _push_native_read_only_row(strings.misc.column_count.c_str(), tileset.column_count);

  _push_native_read_only_row(strings.misc.tile_width.c_str(), tileset.tile_size.x);
  _push_native_read_only_row(strings.misc.tile_height.c_str(), tileset.tile_size.y);
}

void _push_native_tile_properties(const Model& model,
                                  const Strings& strings,
                                  const Entity tile_entity,
                                  Dispatcher& dispatcher)
{
  const auto& context = model.get<Context>(tile_entity);
  const auto& tile = model.get<Tile>(tile_entity);

  _push_native_read_only_row(strings.misc.type.c_str(), strings.misc.tile.c_str());

  if (const auto updated_name = _push_native_name_row(strings, context.name);
      updated_name && !updated_name->empty()) {
    dispatcher.enqueue<RenameTileEvent>(tile.index, *updated_name);
  }

  _push_native_read_only_row(strings.misc.index.c_str(), tile.index);
  _push_native_read_only_row(strings.misc.animated.c_str(),
                             model.has<TileAnimation>(tile_entity));
}

void _push_native_layer_properties(const Model& model,
                                   const Strings& strings,
                                   const Entity layer_entity,
                                   Dispatcher& dispatcher)
{
  const auto& layer = model.get<Layer>(layer_entity);

  switch (layer.type) {
    case LayerType::TileLayer:
      _push_native_read_only_row(strings.misc.type.c_str(),
                                 strings.misc.tile_layer.c_str());
      break;

    case LayerType::ObjectLayer:
      _push_native_read_only_row(strings.misc.type.c_str(),
                                 strings.misc.object_layer.c_str());
      break;

    case LayerType::GroupLayer:
      _push_native_read_only_row(strings.misc.type.c_str(),
                                 strings.misc.group_layer.c_str());
      break;
  }

  _prepare_table_row(strings.misc.opacity.c_str());
  ImGui::TableNextColumn();
  if (const auto value =
          push_float_input(strings, "##Opacity", layer.opacity, 0.0f, 1.0f)) {
    dispatcher.enqueue<SetLayerOpacityEvent>(layer_entity, *value);
  }

  _prepare_table_row(strings.misc.visible.c_str());
  ImGui::TableNextColumn();
  if (const auto value = push_bool_input(strings, "##Visible", layer.visible)) {
    dispatcher.enqueue<SetLayerVisibleEvent>(layer_entity, *value);
  }
}

void _push_native_object_properties(const Model& model,
                                    const Strings& strings,
                                    const Entity object_entity,
                                    Dispatcher& dispatcher)
{
  const auto& context = model.get<Context>(object_entity);
  const auto& object = model.get<Object>(object_entity);

  switch (object.type) {
    case ObjectType::Rect:
      _push_native_read_only_row(strings.misc.type.c_str(),
                                 strings.misc.rectangle.c_str());
      break;

    case ObjectType::Ellipse:
      _push_native_read_only_row(strings.misc.type.c_str(), strings.misc.ellipse.c_str());
      break;

    case ObjectType::Point:
      _push_native_read_only_row(strings.misc.type.c_str(), strings.misc.point.c_str());
      break;
  }

  if (const auto name = _push_native_name_row(strings, context.name)) {
    dispatcher.enqueue<SetObjectNameEvent>(object_entity, *name);
  }

  _push_native_read_only_row("X", object.position.x);
  _push_native_read_only_row("Y", object.position.y);

  if (object.type != ObjectType::Point) {
    _push_native_read_only_row(strings.misc.width.c_str(), object.size.x);
    _push_native_read_only_row(strings.misc.height.c_str(), object.size.y);
  }

  _prepare_table_row(strings.misc.visible.c_str());
  ImGui::TableNextColumn();
  if (const auto visible = push_bool_input(strings, "##Visible", object.visible)) {
    dispatcher.enqueue<SetObjectVisibleEvent>(object_entity, *visible);
  }

  _prepare_table_row(strings.misc.tag.c_str());

  ImGui::TableNextColumn();
  if (const auto tag = push_string_input(strings, "##Tag", object.tag)) {
    dispatcher.enqueue<SetObjectTagEvent>(object_entity, *tag);
  }
}

void _push_custom_properties(const Strings& strings,
                             const Entity context_entity,
                             const Context& context,
                             Dispatcher& dispatcher,
                             bool& is_item_context_open)
{
  bool first = true;

  for (const auto& [name, value]: context.props) {
    const Scope scope {name.c_str()};

    ImGui::TableNextRow();
    ImGui::TableNextColumn();

    if (first) {
      ImGui::Separator();
    }

    ImGui::AlignTextToFramePadding();
    Selectable::property(name.c_str());

    if (!is_item_context_open) {
      is_item_context_open = _push_property_item_context_menu(strings,
                                                              context_entity,
                                                              name,
                                                              gDockState.context_state,
                                                              dispatcher);
    }

    if (gDockState.context_state.show_rename_dialog && !gDockState.rename_target) {
      gDockState.rename_target = name;
    }

    if (gDockState.context_state.show_change_type_dialog &&
        !gDockState.change_type_target) {
      gDockState.change_type_target = name;
    }

    ImGui::TableNextColumn();

    if (first) {
      ImGui::Separator();
    }

    if (auto updated = push_attribute_input(strings, "##CustomPropertyInput", value)) {
      dispatcher.enqueue<UpdatePropertyEvent>(context_entity, name, std::move(*updated));
    }

    first = false;
  }
}

void _push_property_table(const Model& model,
                          const Strings& strings,
                          Dispatcher& dispatcher)
{
  const auto flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
                     ImGuiTableFlags_ScrollY | ImGuiTableFlags_PadOuterX;

  const auto document_entity = sys::get_active_document(model);
  const auto& document = model.get<Document>(document_entity);

  const auto context_entity = document.get_active_context();
  const auto& context = model.get<Context>(context_entity);

  if (const Table table {"##PropertyTable", 2, flags}; table.is_open()) {
    if (model.has<Map>(context_entity)) {
      _push_native_map_properties(model, strings, context_entity, dispatcher);
    }
    else if (model.has<Layer>(context_entity)) {
      _push_native_layer_properties(model, strings, context_entity, dispatcher);
    }
    else if (model.has<Object>(context_entity)) {
      _push_native_object_properties(model, strings, context_entity, dispatcher);
    }
    else if (model.has<Tileset>(context_entity)) {
      _push_native_tileset_properties(model, strings, context_entity, dispatcher);
    }
    else if (model.has<Tile>(context_entity)) {
      _push_native_tile_properties(model, strings, context_entity, dispatcher);
    }

    bool is_item_context_open = false;
    _push_custom_properties(strings,
                            context_entity,
                            context,
                            dispatcher,
                            is_item_context_open);

    if (!is_item_context_open) {
      if (auto popup = Popup::for_window("##PropertyTablePopup"); popup.is_open()) {
        gDockState.context_state.show_add_dialog =
            ImGui::MenuItem(strings.action.create_property.c_str());
      }
    }
  }

  if (gDockState.context_state.show_add_dialog) {
    dispatcher.enqueue<ShowNewPropertyDialogEvent>();
    gDockState.context_state.show_add_dialog = false;
  }

  if (gDockState.context_state.show_rename_dialog) {
    dispatcher.enqueue<ShowRenamePropertyDialogEvent>(gDockState.rename_target.value());
    gDockState.rename_target.reset();
    gDockState.context_state.show_rename_dialog = false;
  }

  if (gDockState.context_state.show_change_type_dialog) {
    const auto& target_name = gDockState.change_type_target.value();
    const auto type = lookup_in(context.props, target_name).get_type();
    dispatcher.enqueue<ShowChangePropertyTypeDialogEvent>(target_name, type);

    gDockState.change_type_target.reset();
    gDockState.context_state.show_change_type_dialog = false;
  }
}

}  // namespace

void show_property_dock(const Model& model, Entity, Dispatcher& dispatcher)
{
  const auto& settings = model.get<Settings>();

  if (!settings.test_flag(SETTINGS_SHOW_PROPERTY_DOCK_BIT)) {
    return;
  }

  const auto& strings = sys::get_current_language_strings(model);

  bool show_property_dock = true;
  const Window window {strings.window.property_dock.c_str(),
                       ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar,
                       &show_property_dock};

  if (show_property_dock != settings.test_flag(SETTINGS_SHOW_PROPERTY_DOCK_BIT)) {
    dispatcher.enqueue<SetFlagSettingEvent>(SETTINGS_SHOW_PROPERTY_DOCK_BIT,
                                            show_property_dock);
  }

  gDockState.has_focus = window.has_focus();

  if (window.is_open()) {
    _push_property_table(model, strings, dispatcher);

    update_add_property_dialog(model, dispatcher);
    update_rename_property_dialog(model, dispatcher);
    update_change_property_type_dialog(model, dispatcher);
  }
}

auto is_property_dock_focused() -> bool
{
  return gDockState.has_focus;
}

}  // namespace tactile::ui

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

#include <imgui.h>

#include "io/compression.hpp"
#include "model/contexts/context_components.hpp"
#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/events/file_events.hpp"
#include "model/events/layer_events.hpp"
#include "model/events/map_events.hpp"
#include "model/events/object_events.hpp"
#include "model/events/property_events.hpp"
#include "model/events/setting_events.hpp"
#include "model/events/tileset_events.hpp"
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"
#include "model/layers/layer_components.hpp"
#include "model/maps/map_components.hpp"
#include "model/objects/object_components.hpp"
#include "model/registry.hpp"
#include "model/tiles/tile_components.hpp"
#include "model/tiles/tile_format_ops.hpp"
#include "model/tilesets/tileset_components.hpp"
#include "model/tilesets/tileset_ops.hpp"
#include "ui/dock/property/dialogs/new_property_dialog.hpp"
#include "ui/dock/property/dialogs/rename_property_dialog.hpp"
#include "ui/dock/property/dialogs/set_property_type_dialog.hpp"
#include "ui/filename_filter.hpp"
#include "ui/widget/attribute_widgets.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto _push_property_item_context_menu(const Strings& strings,
                                                    const Entity context_entity,
                                                    const String& property_name,
                                                    PropertyItemContextMenuState& state,
                                                    Dispatcher& dispatcher) -> bool
{
  if (auto popup = ui::Popup::for_item("##PropertyItemPopup"); popup.is_open()) {
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
    return ui::push_string_input(strings,
                                 "##NativeNameRowInput",
                                 name,
                                 nullptr,
                                 flags,
                                 &ui::filename_filter);
  }
  else {
    return ui::push_string_input(strings, "##NativeNameRowInput", name, nullptr, flags);
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

  const ui::Disable disable;
  bool v = value;
  ImGui::Checkbox("##Bool", &v);
}

void _push_native_map_properties(ModelView& model,
                                 const Strings& strings,
                                 const Entity map_entity)
{
  const auto& registry = model.get_registry();
  const auto& context = registry.get<Context>(map_entity);
  const auto& map = registry.get<Map>(map_entity);
  const auto& tile_format = registry.get<TileFormat>(map_entity);

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
  if (const ui::Combo combo {"##TileEncoding", encoding}; combo.is_open()) {
    if (ui::Selectable::property(strings.misc.plain_encoding.c_str())) {
      model.enqueue<SetTileFormatEncodingEvent>(TileEncoding::Plain);
    }

    if (ui::Selectable::property("Base64")) {
      model.enqueue<SetTileFormatEncodingEvent>(TileEncoding::Base64);
    }
  }

  _prepare_table_row(strings.misc.tile_compression.c_str());
  ImGui::TableNextColumn();

  {
    const ui::Disable disable_if {!supports_compression(tile_format)};

    auto compression = strings.misc.none;
    if (tile_format.compression == TileCompression::Zlib) {
      compression = "Zlib";
    }
    else if (tile_format.compression == TileCompression::Zstd) {
      compression = "Zstd";
    }

    if (const ui::Combo combo {"##TileCompression", compression.c_str()};
        combo.is_open()) {
      if (ui::Selectable::property(strings.misc.none.c_str())) {
        model.enqueue<SetTileFormatCompressionEvent>(TileCompression::None);
      }

      if (ui::Selectable::property("Zlib")) {
        model.enqueue<SetTileFormatCompressionEvent>(TileCompression::Zlib);
      }

      if (ui::Selectable::property("Zstd")) {
        model.enqueue<SetTileFormatCompressionEvent>(TileCompression::Zstd);
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
        model.enqueue<SetZlibCompressionLevelEvent>(level);
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
        model.enqueue<SetZstdCompressionLevelEvent>(level);
      }
    }
  }
}

void _push_native_tileset_properties(ModelView& model,
                                     const Strings& strings,
                                     const Entity tileset_entity)
{
  const auto& registry = model.get_registry();
  const auto& context = registry.get<Context>(tileset_entity);
  const auto& tileset = registry.get<Tileset>(tileset_entity);

  _push_native_read_only_row(strings.misc.type.c_str(), strings.misc.tileset.c_str());

  if (const auto updated_name = _push_native_name_row(strings, context.name, true);
      updated_name && !updated_name->empty()) {
    model.enqueue<RenameTilesetEvent>(tileset_entity, *updated_name);
  }

  _push_native_read_only_row(strings.misc.tile_count.c_str(), sys::tile_count(tileset));
  _push_native_read_only_row(strings.misc.column_count.c_str(), tileset.column_count);

  _push_native_read_only_row(strings.misc.tile_width.c_str(), tileset.tile_size.x);
  _push_native_read_only_row(strings.misc.tile_height.c_str(), tileset.tile_size.y);
}

void _push_native_tile_properties(ModelView& model,
                                  const Strings& strings,
                                  const Entity tile_entity)
{
  const auto& registry = model.get_registry();
  const auto& context = registry.get<Context>(tile_entity);
  const auto& tile = registry.get<Tile>(tile_entity);

  _push_native_read_only_row(strings.misc.type.c_str(), strings.misc.tile.c_str());

  if (const auto updated_name = _push_native_name_row(strings, context.name);
      updated_name && !updated_name->empty()) {
    model.enqueue<RenameTileEvent>(tile_entity, *updated_name);
  }

  _push_native_read_only_row(strings.misc.index.c_str(), tile.index);
  _push_native_read_only_row(strings.misc.animated.c_str(),
                             registry.has<TileAnimation>(tile_entity));
}

void _push_native_layer_properties(ModelView& model,
                                   const Strings& strings,
                                   const Entity layer_entity)
{
  const auto& registry = model.get_registry();
  const auto& layer = registry.get<Layer>(layer_entity);

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
          ui::push_float_input(strings, "##Opacity", layer.opacity, 0.0f, 1.0f)) {
    model.enqueue<SetLayerOpacityEvent>(layer_entity, *value);
  }

  _prepare_table_row(strings.misc.visible.c_str());
  ImGui::TableNextColumn();
  if (const auto value = ui::push_bool_input(strings, "##Visible", layer.visible)) {
    model.enqueue<SetLayerVisibleEvent>(layer_entity, *value);
  }
}

void _push_native_object_properties(ModelView& model,
                                    const Strings& strings,
                                    const Entity object_entity)
{
  const auto& registry = model.get_registry();
  const auto& context = registry.get<Context>(object_entity);
  const auto& object = registry.get<Object>(object_entity);

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
    model.enqueue<SetObjectNameEvent>(object_entity, *name);
  }

  _push_native_read_only_row("X", object.position.x);
  _push_native_read_only_row("Y", object.position.y);

  if (object.type != ObjectType::Point) {
    _push_native_read_only_row(strings.misc.width.c_str(), object.size.x);
    _push_native_read_only_row(strings.misc.height.c_str(), object.size.y);
  }

  _prepare_table_row(strings.misc.visible.c_str());
  ImGui::TableNextColumn();
  if (const auto visible = ui::push_bool_input(strings, "##Visible", object.visible)) {
    model.enqueue<SetObjectVisibleEvent>(object_entity, *visible);
  }

  _prepare_table_row(strings.misc.tag.c_str());

  ImGui::TableNextColumn();
  if (const auto tag = ui::push_string_input(strings, "##Tag", object.tag)) {
    model.enqueue<SetObjectTagEvent>(object_entity, *tag);
  }
}

void _push_custom_properties(const Strings& strings,
                             const Entity context_entity,
                             const Context& context,
                             PropertyDockState& state,
                             Dispatcher& dispatcher,
                             bool& is_item_context_open)
{
  bool first = true;

  for (const auto& [name, value]: context.props) {
    const ui::Scope scope {name.c_str()};

    ImGui::TableNextRow();
    ImGui::TableNextColumn();

    if (first) {
      ImGui::Separator();
    }

    ImGui::AlignTextToFramePadding();
    ui::Selectable::property(name.c_str());

    if (!is_item_context_open) {
      is_item_context_open = _push_property_item_context_menu(strings,
                                                              context_entity,
                                                              name,
                                                              state.context_state,
                                                              dispatcher);
    }

    if (state.context_state.show_rename_dialog && !state.rename_target) {
      state.rename_target = name;
    }

    if (state.context_state.show_change_type_dialog && !state.change_type_target) {
      state.change_type_target = name;
    }

    ImGui::TableNextColumn();

    if (first) {
      ImGui::Separator();
    }

    if (auto updated =
            ui::push_attribute_input(strings, "##CustomPropertyInput", value)) {
      dispatcher.enqueue<UpdatePropertyEvent>(context_entity, name, std::move(*updated));
    }

    first = false;
  }
}

void _push_property_table(ModelView& model,
                          const Strings& strings,
                          PropertyDockState& state)
{
  const auto& registry = model.get_registry();
  const auto context_entity = sys::get_active_context(registry);

  const auto table_flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
                           ImGuiTableFlags_ScrollY | ImGuiTableFlags_PadOuterX;
  if (const ui::Table table {"##PropertyTable", 2, table_flags}; table.is_open()) {
    if (registry.has<Map>(context_entity)) {
      _push_native_map_properties(model, strings, context_entity);
    }
    else if (registry.has<Layer>(context_entity)) {
      _push_native_layer_properties(model, strings, context_entity);
    }
    else if (registry.has<Object>(context_entity)) {
      _push_native_object_properties(model, strings, context_entity);
    }
    else if (registry.has<Tileset>(context_entity)) {
      _push_native_tileset_properties(model, strings, context_entity);
    }
    else if (registry.has<Tile>(context_entity)) {
      _push_native_tile_properties(model, strings, context_entity);
    }

    const auto& context = registry.get<Context>(context_entity);

    bool is_item_context_open = false;
    _push_custom_properties(strings,
                            context_entity,
                            context,
                            state,
                            model.get_dispatcher(),
                            is_item_context_open);

    if (!is_item_context_open) {
      if (auto popup = ui::Popup::for_window("##PropertyTablePopup"); popup.is_open()) {
        state.context_state.show_add_dialog =
            ImGui::MenuItem(strings.action.create_property.c_str());
      }
    }
  }

  if (state.context_state.show_add_dialog) {
    model.enqueue<ShowNewPropertyDialogEvent>(context_entity);
    state.context_state.show_add_dialog = false;
  }

  if (state.context_state.show_rename_dialog) {
    model.enqueue<ShowRenamePropertyDialogEvent>(context_entity,
                                                 state.rename_target.value());
    state.rename_target.reset();
    state.context_state.show_rename_dialog = false;
  }

  if (state.context_state.show_change_type_dialog) {
    const auto& property_name = state.change_type_target.value();
    model.enqueue<ShowSetPropertyTypeDialogEvent>(context_entity, property_name);

    state.change_type_target.reset();
    state.context_state.show_change_type_dialog = false;
  }
}

}  // namespace

void push_property_dock_widget(ModelView& model, PropertyDockState& state)
{
  const auto& strings = model.get_language_strings();
  const auto& settings = model.get_settings();

  if (!settings.test_flag(SETTINGS_SHOW_PROPERTY_DOCK_BIT)) {
    return;
  }

  bool show_property_dock = true;
  const ui::Window window {strings.window.property_dock.c_str(),
                           ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar,
                           &show_property_dock};

  if (show_property_dock != settings.test_flag(SETTINGS_SHOW_PROPERTY_DOCK_BIT)) {
    model.enqueue<SetFlagSettingEvent>(SETTINGS_SHOW_PROPERTY_DOCK_BIT,
                                       show_property_dock);
  }

  state.has_focus = window.has_focus();

  if (window.is_open()) {
    _push_property_table(model, strings, state);

    push_new_property_dialog(model, state.new_property_dialog);
    push_rename_property_dialog(model, state.rename_property_dialog);
    push_set_property_type_dialog(model, state.set_property_type_dialog);
  }
}

}  // namespace tactile

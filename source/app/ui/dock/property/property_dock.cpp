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

#include "core/context/context_visitor.hpp"
#include "core/layer/group_layer.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/map.hpp"
#include "core/tile/tileset.hpp"
#include "core/type/maybe.hpp"
#include "core/type/uuid.hpp"
#include "io/proto/preferences.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/tileset_document.hpp"
#include "model/event/document_events.hpp"
#include "model/event/layer_events.hpp"
#include "model/event/map_events.hpp"
#include "model/event/object_events.hpp"
#include "model/event/property_events.hpp"
#include "model/event/tileset_events.hpp"
#include "model/model.hpp"
#include "ui/dock/property/dialogs/add_property_dialog.hpp"
#include "ui/dock/property/dialogs/change_property_type_dialog.hpp"
#include "ui/dock/property/dialogs/rename_property_dialog.hpp"
#include "ui/filename_filter.hpp"
#include "ui/widget/input_widgets.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {
namespace {

struct PropertyItemContextMenuState final {
  bool show_add_dialog         : 1 {};
  bool show_rename_dialog      : 1 {};
  bool show_change_type_dialog : 1 {};
};

inline PropertyItemContextMenuState context_state;
inline Maybe<String> rename_target;
inline Maybe<String> change_type_target;
constinit bool is_focused = false;

[[nodiscard]] auto property_item_context_menu(const UUID& context_id,
                                              entt::dispatcher& dispatcher,
                                              const String& name,
                                              PropertyItemContextMenuState& state) -> bool
{
  const auto& lang = get_current_language();

  if (auto popup = Popup::for_item("##PropertyItemPopup"); popup.is_open()) {
    state.show_rename_dialog = ImGui::MenuItem(lang.action.rename_property.c_str());
    state.show_change_type_dialog =
        ImGui::MenuItem(lang.action.change_property_type.c_str());

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.remove_property.c_str())) {
      dispatcher.enqueue<RemovePropertyEvent>(context_id, name);
    }

    return true;
  }
  else {
    return false;
  }
}

void prepare_table_row(const char* label)
{
  ImGui::TableNextRow();
  ImGui::TableNextColumn();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(label);
}

[[nodiscard]] auto native_name_row(const String& name,
                                   const bool validate_as_file_name = false)
    -> Maybe<String>
{
  const auto& lang = get_current_language();
  prepare_table_row(lang.misc.name.c_str());

  ImGui::TableNextColumn();

  auto flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;
  if (validate_as_file_name) {
    flags |= ImGuiInputTextFlags_CallbackCharFilter;
    return input_string("##NativeNameRowInput", name, nullptr, flags, filename_filter);
  }
  else {
    return input_string("##NativeNameRowInput", name, nullptr, flags);
  }
}

void native_read_only_row(const char* label, const char* value)
{
  prepare_table_row(label);

  ImGui::TableNextColumn();
  ImGui::TextUnformatted(value);
}

void native_read_only_row(const char* label, const float value)
{
  prepare_table_row(label);

  ImGui::TableNextColumn();
  ImGui::Text("%.2f", value);
}

void native_read_only_row(const char* label, const int32 value)
{
  prepare_table_row(label);

  ImGui::TableNextColumn();
  ImGui::Text("%d", value);
}

void native_read_only_row(const char* label, const usize value)
{
  prepare_table_row(label);

  ImGui::TableNextColumn();
  ImGui::Text("%llu", static_cast<ulonglong>(value)); /* Cast to avoid format warnings */
}

void show_native_map_properties(const Map& map, entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  native_read_only_row(lang.misc.type.c_str(), lang.misc.map.c_str());
  native_read_only_row(lang.misc.name.c_str(), map.get_ctx().name().c_str());

  native_read_only_row(lang.misc.tile_width.c_str(), map.tile_size().x);
  native_read_only_row(lang.misc.tile_height.c_str(), map.tile_size().y);

  native_read_only_row(lang.misc.row_count.c_str(), map.row_count());
  native_read_only_row(lang.misc.column_count.c_str(), map.column_count());

  prepare_table_row(lang.misc.tile_encoding.c_str());
  ImGui::TableNextColumn();

  const auto& format = map.tile_format();

  const auto* encoding = format.encoding() == TileEncoding::Plain
                             ? lang.misc.plain_encoding.c_str()
                             : "Base64";
  if (Combo combo {"##TileEncoding", encoding}; combo.is_open()) {
    if (Selectable::Property(lang.misc.plain_encoding.c_str())) {
      dispatcher.enqueue<SetTileFormatEncodingEvent>(TileEncoding::Plain);
    }

    if (Selectable::Property("Base64")) {
      dispatcher.enqueue<SetTileFormatEncodingEvent>(TileEncoding::Base64);
    }
  }

  prepare_table_row(lang.misc.tile_compression.c_str());
  ImGui::TableNextColumn();

  {
    Disable disable_if_cannot_compress {!format.supports_any_compression()};

    auto compression = lang.misc.none;
    if (format.compression() == TileCompression::Zlib) {
      compression = "Zlib";
    }
    else if (format.compression() == TileCompression::Zstd) {
      compression = "Zstd";
    }

    if (Combo combo {"##TileCompression", compression.c_str()}; combo.is_open()) {
      if (Selectable::Property(lang.misc.none.c_str())) {
        dispatcher.enqueue<SetTileFormatCompressionEvent>(TileCompression::None);
      }

      if (Selectable::Property("Zlib")) {
        dispatcher.enqueue<SetTileFormatCompressionEvent>(TileCompression::Zlib);
      }

      if (Selectable::Property("Zstd")) {
        dispatcher.enqueue<SetTileFormatCompressionEvent>(TileCompression::Zstd);
      }
    }

    if (format.compression() == TileCompression::Zlib) {
      prepare_table_row(lang.misc.compression_level.c_str());
      ImGui::TableNextColumn();

      auto level = format.zlib_compression_level();
      if (ImGui::SliderInt("##CompressionLevel",
                           &level,
                           TileFormat::min_zlib_compression_level(),
                           TileFormat::max_zlib_compression_level())) {
        dispatcher.enqueue<SetZlibCompressionLevelEvent>(level);
      }
    }
    else if (format.compression() == TileCompression::Zstd) {
      prepare_table_row(lang.misc.compression_level.c_str());
      ImGui::TableNextColumn();

      auto level = format.zstd_compression_level();
      if (ImGui::SliderInt("##CompressionLevel",
                           &level,
                           TileFormat::min_zstd_compression_level(),
                           TileFormat::max_zstd_compression_level())) {
        dispatcher.enqueue<SetZstdCompressionLevelEvent>(level);
      }
    }
  }
}

void show_native_tileset_properties(const Tileset& tileset, entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  native_read_only_row(lang.misc.type.c_str(), lang.misc.tileset.c_str());

  if (const auto updated_name = native_name_row(tileset.get_ctx().name(), true);
      updated_name && !updated_name->empty()) {
    dispatcher.enqueue<RenameTilesetEvent>(tileset.get_uuid(), *updated_name);
  }

  native_read_only_row(lang.misc.tile_count.c_str(), tileset.tile_count());
  native_read_only_row(lang.misc.column_count.c_str(), tileset.column_count());

  native_read_only_row(lang.misc.tile_width.c_str(), tileset.tile_size().x);
  native_read_only_row(lang.misc.tile_height.c_str(), tileset.tile_size().y);
}

void show_native_layer_properties(const Layer& layer, entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  switch (layer.get_type()) {
    case LayerType::TileLayer:
      native_read_only_row(lang.misc.type.c_str(), lang.misc.tile_layer.c_str());
      break;

    case LayerType::ObjectLayer:
      native_read_only_row(lang.misc.type.c_str(), lang.misc.object_layer.c_str());
      break;

    case LayerType::GroupLayer:
      native_read_only_row(lang.misc.type.c_str(), lang.misc.group_layer.c_str());
      break;
  }

  prepare_table_row(lang.misc.opacity.c_str());
  ImGui::TableNextColumn();
  if (const auto value = input_float("##Opacity", layer.get_opacity(), 0.0f, 1.0f)) {
    dispatcher.enqueue<SetLayerOpacityEvent>(layer.get_uuid(), *value);
  }

  prepare_table_row(lang.misc.visible.c_str());
  ImGui::TableNextColumn();
  if (const auto value = input_bool("##Visible", layer.is_visible())) {
    dispatcher.enqueue<SetLayerVisibleEvent>(layer.get_uuid(), *value);
  }
}

void show_native_object_properties(const Object& object, entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  switch (object.get_type()) {
    case ObjectType::Rect:
      native_read_only_row(lang.misc.type.c_str(), lang.misc.rectangle.c_str());
      break;

    case ObjectType::Ellipse:
      native_read_only_row(lang.misc.type.c_str(), lang.misc.ellipse.c_str());
      break;

    case ObjectType::Point:
      native_read_only_row(lang.misc.type.c_str(), lang.misc.point.c_str());
      break;
  }

  if (const auto updated_name = native_name_row(object.get_ctx().name())) {
    dispatcher.enqueue<SetObjectNameEvent>(object.get_uuid(), *updated_name);
  }

  const auto& pos = object.get_pos();
  native_read_only_row("X", pos.x);
  native_read_only_row("Y", pos.y);

  if (object.get_type() != ObjectType::Point) {
    const auto& size = object.get_size();
    native_read_only_row(lang.misc.width.c_str(), size.x);
    native_read_only_row(lang.misc.height.c_str(), size.y);
  }

  prepare_table_row(lang.misc.visible.c_str());
  ImGui::TableNextColumn();
  if (const auto visible = input_bool("##Visible", object.is_visible())) {
    dispatcher.enqueue<SetObjectVisibleEvent>(object.get_uuid(), *visible);
  }

  prepare_table_row(lang.misc.tag.c_str());

  ImGui::TableNextColumn();
  if (const auto tag = input_string("##Tag", object.get_tag())) {
    dispatcher.enqueue<SetObjectTagEvent>(object.get_uuid(), *tag);
  }
}

void show_custom_properties(const Context& context,
                            entt::dispatcher& dispatcher,
                            bool& is_item_context_open)
{
  bool first = true;

  for (const auto& [name, value]: context.get_ctx().props()) {
    const Scope scope {name.c_str()};

    ImGui::TableNextRow();
    ImGui::TableNextColumn();

    if (first) {
      ImGui::Separator();
    }

    ImGui::AlignTextToFramePadding();
    Selectable::Property(name.c_str());

    if (!is_item_context_open) {
      is_item_context_open =
          property_item_context_menu(context.get_uuid(), dispatcher, name, context_state);
    }

    if (context_state.show_rename_dialog && !rename_target) {
      rename_target = name;
    }

    if (context_state.show_change_type_dialog && !change_type_target) {
      change_type_target = name;
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

struct ContextPropertyVisitor final : ContextVisitor {
  entt::dispatcher* dispatcher {};

  explicit ContextPropertyVisitor(entt::dispatcher& dispatcher)
      : dispatcher {&dispatcher}
  {
  }

  void visit(const Map& map) override { show_native_map_properties(map, *dispatcher); }

  void visit(const TileLayer& layer) override
  {
    show_native_layer_properties(layer, *dispatcher);
  }

  void visit(const ObjectLayer& layer) override
  {
    show_native_layer_properties(layer, *dispatcher);
  }

  void visit(const GroupLayer& layer) override
  {
    show_native_layer_properties(layer, *dispatcher);
  }

  void visit(const Object& object) override
  {
    show_native_object_properties(object, *dispatcher);
  }

  void visit(const Tileset& tileset) override
  {
    show_native_tileset_properties(tileset, *dispatcher);
  }

  void visit(const Tile&) override
  {
    // TODO
  }
};

void update_property_table(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  constexpr auto flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
                         ImGuiTableFlags_ScrollY | ImGuiTableFlags_PadOuterX;

  const auto& lang = get_current_language();
  const auto& document = model.require_active_document();
  const auto& context = document.get_contexts().active_context();

  if (Table table {"##PropertyTable", 2, flags}; table.is_open()) {
    ContextPropertyVisitor visitor {dispatcher};
    context.accept(visitor);

    bool is_item_context_open = false;
    show_custom_properties(context, dispatcher, is_item_context_open);

    if (!is_item_context_open) {
      if (auto popup = Popup::for_window("##PropertyTablePopup"); popup.is_open()) {
        context_state.show_add_dialog =
            ImGui::MenuItem(lang.action.create_property.c_str());
      }
    }
  }

  if (context_state.show_add_dialog) {
    dispatcher.enqueue<ShowAddPropertyDialogEvent>();
    context_state.show_add_dialog = false;
  }

  if (context_state.show_rename_dialog) {
    dispatcher.enqueue<ShowRenamePropertyDialogEvent>(rename_target.value());
    rename_target.reset();
    context_state.show_rename_dialog = false;
  }

  if (context_state.show_change_type_dialog) {
    const auto& target_name = change_type_target.value();
    const auto type = context.get_ctx().props().at(target_name).type();
    dispatcher.enqueue<ShowChangePropertyTypeDialogEvent>(target_name, type);
    change_type_target.reset();
    context_state.show_change_type_dialog = false;
  }
}

}  // namespace

void update_property_dock(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  auto& prefs = io::get_preferences();

  if (!prefs.show_property_dock) {
    return;
  }

  const auto& lang = get_current_language();

  const Window window {lang.window.property_dock.c_str(),
                       ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar,
                       &prefs.show_property_dock};
  is_focused = window.has_focus();

  if (window.is_open()) {
    update_property_table(model, dispatcher);

    update_add_property_dialog(model, dispatcher);
    update_rename_property_dialog(model, dispatcher);
    update_change_property_type_dialog(model, dispatcher);
  }
}

auto is_property_dock_focused() -> bool
{
  return is_focused;
}

}  // namespace tactile::ui

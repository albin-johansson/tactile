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

#include "property_table.hpp"

#include <locale>  // locale, isalpha, isdigit, isspace

#include <imgui.h>

#include "core/components/attributes.hpp"
#include "core/components/layers.hpp"
#include "core/components/objects.hpp"
#include "core/components/tiles.hpp"
#include "core/map.hpp"
#include "core/systems/context_system.hpp"
#include "core/systems/property_system.hpp"
#include "editor/events/layer_events.hpp"
#include "editor/events/object_events.hpp"
#include "editor/events/property_events.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/gui/common/input_widgets.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "meta/build.hpp"
#include "tactile.hpp"

namespace tactile {
namespace {

void PrepareTableRow(const c_str label)
{
  ImGui::TableNextRow();
  ImGui::TableNextColumn();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(label);
}

[[nodiscard]] auto NativeNameRow(const std::string& name,
                                 const bool validateAsFileName = false)
    -> maybe<std::string>
{
  PrepareTableRow("Name");

  ImGui::TableNextColumn();

  auto flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;
  if (validateAsFileName) {
    flags |= ImGuiInputTextFlags_CallbackCharFilter;

    /* This is a basic filter for only allowing a basic subset of characters, in order to
       guarantee that the user provides names that are usable as file names. */
    const auto filter = [](ImGuiInputTextCallbackData* data) -> int {
      const auto& locale = std::locale::classic();
      const auto ch = static_cast<wchar_t>(data->EventChar);

      if (std::isalpha(ch, locale) || std::isdigit(ch, locale) ||
          std::isspace(ch, locale) || ch == '-' || ch == '_') {
        return 0;  // Accept the character
      }
      else {
        return 1;  // Reject the character
      }
    };

    return input_string("##NativeNameRowInput", name, nullptr, flags, filter);
  }
  else {
    return input_string("##NativeNameRowInput", name, nullptr, flags);
  }
}

[[nodiscard]] auto NativeOpacityRow(const float opacity) -> maybe<float>
{
  PrepareTableRow("Opacity");

  ImGui::TableNextColumn();
  return input_float("##NativeOpacityRow", opacity, 0.0f, 1.0f);
}

[[nodiscard]] auto NativeVisibilityRow(const bool visible) -> maybe<bool>
{
  PrepareTableRow("Visible");

  ImGui::TableNextColumn();
  return input_bool("##NativeVisibilityRow", visible);
}

void NativeReadOnlyRow(const c_str label, const c_str value)
{
  PrepareTableRow(label);

  ImGui::TableNextColumn();
  ImGui::TextUnformatted(value);
}

void NativeReadOnlyRow(const c_str label, const float value)
{
  PrepareTableRow(label);

  ImGui::TableNextColumn();
  ImGui::Text("%.2f", value);
}

void NativeReadOnlyRow(const c_str label, const int32 value)
{
  PrepareTableRow(label);

  ImGui::TableNextColumn();
  ImGui::Text("%d", value);
}

void NativeReadOnlyRow(const c_str label, const usize value)
{
  PrepareTableRow(label);

  ImGui::TableNextColumn();
  ImGui::Text("%llu", static_cast<ulonglong>(value)); /* Cast to avoid format warnings */
}

void ShowNativeMapProperties(const std::string& name, const MapInfo& map)
{
  NativeReadOnlyRow("Type", "Map");
  NativeReadOnlyRow("Name", name.c_str());

  NativeReadOnlyRow("Tile width", map.tile_width);
  NativeReadOnlyRow("Tile height", map.tile_height);

  NativeReadOnlyRow("Row count", map.row_count);
  NativeReadOnlyRow("Column count", map.column_count);
}

void ShowNativeTilesetProperties(const std::string& name,
                                 const comp::Tileset& tileset,
                                 entt::dispatcher& dispatcher)
{
  NativeReadOnlyRow("Type", "Tileset");

  if constexpr (is_debug_build) {
    NativeReadOnlyRow("ID", tileset.id);
  }

  if (const auto updatedName = NativeNameRow(name, true);
      updatedName && !updatedName->empty()) {
    dispatcher.enqueue<SetTilesetNameEvent>(tileset.id, *updatedName);
  }

  NativeReadOnlyRow("First tile ID", tileset.first_id);
  NativeReadOnlyRow("Last tile ID", tileset.last_id);

  NativeReadOnlyRow("Tile count", tileset.tile_count);
  NativeReadOnlyRow("Column count", tileset.column_count);

  NativeReadOnlyRow("Tile width", tileset.tile_width);
  NativeReadOnlyRow("Tile height", tileset.tile_height);
}

void ShowNativeLayerProperties(const comp::Layer& layer, entt::dispatcher& dispatcher)
{
  switch (layer.type) {
    case layer_type::tile_layer:
      NativeReadOnlyRow("Type", "Tile Layer");
      break;

    case layer_type::object_layer:
      NativeReadOnlyRow("Type", "Object Layer");
      break;

    case layer_type::group_layer:
      NativeReadOnlyRow("Type", "Group Layer");
      break;
  }

  if constexpr (is_debug_build) {
    NativeReadOnlyRow("ID", layer.id);
  }

  if (const auto value = NativeOpacityRow(layer.opacity)) {
    dispatcher.enqueue<SetLayerOpacityEvent>(layer.id, *value);
  }

  if (const auto value = NativeVisibilityRow(layer.visible)) {
    dispatcher.enqueue<SetLayerVisibleEvent>(layer.id, *value);
  }
}

void ShowNativeObjectProperties(const std::string& name,
                                const comp::Object& object,
                                entt::dispatcher& dispatcher)
{
  switch (object.type) {
    case object_type::rect:
      NativeReadOnlyRow("Type", "Rectangle");
      break;

    case object_type::point:
      NativeReadOnlyRow("Type", "Point");
      break;

    case object_type::ellipse:
      NativeReadOnlyRow("Type", "Ellipse");
      break;
  }

  if constexpr (is_debug_build) {
    NativeReadOnlyRow("ID", object.id);
  }

  if (const auto updatedName = NativeNameRow(name)) {
    dispatcher.enqueue<SetObjectNameEvent>(object.id, *updatedName);
  }

  NativeReadOnlyRow("X", object.x);
  NativeReadOnlyRow("Y", object.y);

  if (object.type != object_type::point) {
    NativeReadOnlyRow("Width", object.width);
    NativeReadOnlyRow("Height", object.height);
  }

  if (const auto visible = NativeVisibilityRow(object.visible)) {
    dispatcher.enqueue<SetObjectVisibilityEvent>(object.id, *visible);
  }

  PrepareTableRow("Tag");

  ImGui::TableNextColumn();
  if (const auto tag = input_string("##NativeObjectTagInput", object.tag)) {
    dispatcher.enqueue<SetObjectTagEvent>(object.id, *tag);
  }
}

}  // namespace

void PropertyTable::Update(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  constexpr auto flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
                         ImGuiTableFlags_ScrollY | ImGuiTableFlags_PadOuterX;

  const auto& current = registry.ctx<comp::ActiveAttributeContext>();
  const auto& context = sys::current_context(registry);

  if (scoped::table table{"##PropertyTable", 2, flags}; table.is_open()) {
    if (current.entity == entt::null) {
      ShowNativeMapProperties(context.name, registry.ctx<MapInfo>());
    }
    else {
      if (const auto* tileset = registry.try_get<comp::Tileset>(current.entity)) {
        ShowNativeTilesetProperties(context.name, *tileset, dispatcher);
      }
      else if (const auto* layer = registry.try_get<comp::Layer>(current.entity)) {
        ShowNativeLayerProperties(*layer, dispatcher);
      }
      else if (const auto* object = registry.try_get<comp::Object>(current.entity)) {
        ShowNativeObjectProperties(context.name, *object, dispatcher);
      }
    }

    bool isItemContextOpen = false;
    ShowCustomProperties(registry, dispatcher, context, isItemContextOpen);

    if (!isItemContextOpen) {
      if (auto popup = scoped::popup::for_window("##PropertyTableContext");
          popup.is_open()) {
        mContextState.show_add_dialog =
            ImGui::MenuItem(TAC_ICON_ADD " Add New Property...");
      }
    }
  }

  if (mContextState.show_add_dialog) {
    dispatcher.enqueue<ShowAddPropertyDialogEvent>();
    mContextState.show_add_dialog = false;
  }

  if (mContextState.show_rename_dialog) {
    dispatcher.enqueue<ShowRenamePropertyDialogEvent>(mRenameTarget.value());
    mRenameTarget.reset();
    mContextState.show_rename_dialog = false;
  }

  if (mContextState.show_change_type_dialog) {
    const auto& name = mChangeTypeTarget.value();
    const auto type = sys::get_property(registry, context, name).value.type();
    dispatcher.enqueue<ShowChangePropertyTypeDialogEvent>(name, type);
    mChangeTypeTarget.reset();
    mContextState.show_change_type_dialog = false;
  }
}

void PropertyTable::ShowCustomProperties(const entt::registry& registry,
                                         entt::dispatcher& dispatcher,
                                         const comp::AttributeContext& context,
                                         bool& isItemContextOpen)
{
  bool first = true;

  for (const auto entity : context.properties) {
    const auto& property = registry.get<comp::Property>(entity);

    const auto& name = property.name;
    const auto& value = property.value;

    const scoped::id scope{name.c_str()};

    ImGui::TableNextRow();
    ImGui::TableNextColumn();

    if (first) {
      ImGui::Separator();
    }

    ImGui::AlignTextToFramePadding();
    ImGui::Selectable(name.c_str());

    if (!isItemContextOpen) {
      isItemContextOpen = PropertyItemContextMenu(dispatcher, name, mContextState);
    }

    if (mContextState.show_rename_dialog && !mRenameTarget) {
      mRenameTarget = name;
    }

    if (mContextState.show_change_type_dialog && !mChangeTypeTarget) {
      mChangeTypeTarget = name;
    }

    ImGui::TableNextColumn();

    if (first) {
      ImGui::Separator();
    }

    if (value.is_string()) {
      if (const auto updated = input_string("##CustomPropertyInput", value.as_string())) {
        dispatcher.enqueue<UpdatePropertyEvent>(name, *updated);
      }
    }
    else if (value.is_int()) {
      if (const auto updated = input_int("##CustomPropertyInput", value.as_int())) {
        dispatcher.enqueue<UpdatePropertyEvent>(name, *updated);
      }
    }
    else if (value.is_float()) {
      if (const auto updated = input_float("##CustomPropertyInput", value.as_float())) {
        dispatcher.enqueue<UpdatePropertyEvent>(name, *updated);
      }
    }
    else if (value.is_bool()) {
      if (const auto updated = input_bool("##CustomPropertyInput", value.as_bool())) {
        dispatcher.enqueue<UpdatePropertyEvent>(name, *updated);
      }
    }
    else if (value.is_color()) {
      if (const auto updated = input_color("##CustomPropertyInput", value.as_color())) {
        dispatcher.enqueue<UpdatePropertyEvent>(name, *updated);
      }
    }
    else if (value.is_object()) {
      if (const auto updated = input_object("##CustomPropertyInput", value.as_object())) {
        dispatcher.enqueue<UpdatePropertyEvent>(name, *updated);
      }
    }
    else if (value.is_file()) {
      if (const auto updated = input_file("##CustomPropertyInput", value.as_file())) {
        dispatcher.enqueue<UpdatePropertyEvent>(name, *updated);
      }
    }

    first = false;
  }
}

}  // namespace tactile

#include "property_table.hpp"

#include <string>   // string
#include <utility>  // move

#include <tactile-base/tactile_std.hpp>

#include <imgui.h>

#include "core/components/layer.hpp"
#include "core/components/object.hpp"
#include "core/components/property.hpp"
#include "core/components/tileset.hpp"
#include "core/systems/property_system.hpp"
#include "core/utils/scope_id.hpp"
#include "dialogs/add_property_dialog.hpp"
#include "dialogs/change_property_type_dialog.hpp"
#include "dialogs/rename_property_dialog.hpp"
#include "editor/events/layer_events.hpp"
#include "editor/events/property_events.hpp"
#include "editor/gui/icons.hpp"
#include "items/bool_property_widget.hpp"
#include "items/color_property_widget.hpp"
#include "items/file_property_widget.hpp"
#include "items/float_property_widget.hpp"
#include "items/int_property_widget.hpp"
#include "items/object_property_widget.hpp"
#include "items/property_item_context_menu.hpp"
#include "items/string_property_widget.hpp"

namespace Tactile {
namespace {

constinit PropertyItemContextMenuState context_state;
inline Maybe<std::string> rename_target;
inline Maybe<std::string> change_type_target;

[[nodiscard]] auto NativeNameRow(const std::string& name) -> Maybe<std::string>
{
  ImGui::TableNextRow();
  ImGui::TableNextColumn();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted("Name");

  ImGui::TableNextColumn();
  return StringPropertyWidget(name);
}

[[nodiscard]] auto NativeOpacityRow(const float opacity) -> Maybe<float>
{
  ImGui::TableNextRow();
  ImGui::TableNextColumn();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted("Opacity");

  ImGui::TableNextColumn();
  return FloatPropertyWidget(opacity, 0.0f, 1.0f);
}

[[nodiscard]] auto NativeVisibilityRow(const bool visible) -> Maybe<bool>
{
  ImGui::TableNextRow();
  ImGui::TableNextColumn();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted("Visible");

  ImGui::TableNextColumn();
  return BoolPropertyWidget(visible);
}

void NativeReadOnlyRow(const CStr label, const CStr value)
{
  ImGui::TableNextRow();
  ImGui::TableNextColumn();
  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(label);

  ImGui::TableNextColumn();
  ImGui::BeginDisabled();
  ImGui::TextUnformatted(value);
  ImGui::EndDisabled();
}

void NativeReadOnlyRow(const CStr label, const float value)
{
  ImGui::TableNextRow();
  ImGui::TableNextColumn();
  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(label);

  ImGui::TableNextColumn();
  ImGui::BeginDisabled();
  ImGui::Text("%.2f", value);
  ImGui::EndDisabled();
}

void NativeReadOnlyRow(const CStr label, const int32 value)
{
  ImGui::TableNextRow();
  ImGui::TableNextColumn();
  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(label);

  ImGui::TableNextColumn();
  ImGui::BeginDisabled();
  ImGui::Text("%i", value);
  ImGui::EndDisabled();
}

void ShowNativeTilesetProperties(const Tileset& tileset, entt::dispatcher& dispatcher)
{
  const ScopeID scope{"NativeTilesetProperties"};
  NativeReadOnlyRow("Type", "Tileset");

  NativeReadOnlyRow("First tile ID", tileset.first_id.get());
  NativeReadOnlyRow("Last tile ID", tileset.last_id.get());

  NativeReadOnlyRow("Tile count", tileset.tile_count);
  NativeReadOnlyRow("Column count", tileset.column_count);

  NativeReadOnlyRow("Tile width", tileset.tile_width);
  NativeReadOnlyRow("Tile height", tileset.tile_height);

  ImGui::Separator();
}

void ShowNativeLayerProperties(const Layer& layer, entt::dispatcher& dispatcher)
{
  const ScopeID scope{"NativeLayerProperties"};

  switch (layer.type) {
    case LayerType::TileLayer:
      NativeReadOnlyRow("Type", "Tile Layer");
      break;

    case LayerType::ObjectLayer:
      NativeReadOnlyRow("Type", "Object Layer");
      break;

    case LayerType::GroupLayer:
      NativeReadOnlyRow("Type", "Group Layer");
      break;
  }

  if constexpr (cen::is_debug_build()) {
    ImGui::TableNextRow();
    ImGui::TableNextColumn();

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("ID");

    ImGui::TableNextColumn();
    ImGui::BeginDisabled();
    ImGui::Text("%i", layer.id.get());
    ImGui::EndDisabled();
  }

  if (const auto value = NativeOpacityRow(layer.opacity)) {
    dispatcher.enqueue<SetLayerOpacityEvent>(layer.id, *value);
  }

  if (const auto value = NativeVisibilityRow(layer.visible)) {
    dispatcher.enqueue<SetLayerVisibleEvent>(layer.id, *value);
  }

  ImGui::Separator();
}

void ShowNativeObjectProperties(const Object& object, entt::dispatcher& dispatcher)
{
  const ScopeID scope{"NativeObjectProperties"};

  switch (object.type) {
    case ObjectType::Rectangle:
      NativeReadOnlyRow("Type", "Rectangle");
      break;

    case ObjectType::Point:
      NativeReadOnlyRow("Type", "Point");
      break;

    case ObjectType::Ellipse:
      NativeReadOnlyRow("Type", "Ellipse");
      break;
  }

  NativeReadOnlyRow("X", object.x);
  NativeReadOnlyRow("Y", object.y);

  if (object.type != ObjectType::Point) {
    NativeReadOnlyRow("Width", object.width);
    NativeReadOnlyRow("Height", object.height);
  }

  if (const auto visible = NativeVisibilityRow(object.visible)) {
    // TODO
  }

  ImGui::TableNextRow();
  ImGui::TableNextColumn();
  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted("Tag");

  ImGui::TableNextColumn();
  if (const auto type = StringPropertyWidget(object.custom_type)) {
    // TODO
  }

  ImGui::Separator();
}

void ShowCustomProperties(const entt::registry& registry,
                          entt::dispatcher& dispatcher,
                          const PropertyContext& context,
                          bool& isItemContextOpen)
{
  for (const auto entity : context.properties) {
    const auto& property = registry.get<Property>(entity);

    const auto& name = property.name;
    const auto& value = property.value;

    const ScopeID scope{name.c_str()};

    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::AlignTextToFramePadding();
    ImGui::Selectable(name.c_str());

    if (!isItemContextOpen) {
      isItemContextOpen = PropertyItemContextMenu(dispatcher, name, context_state);
    }

    if (context_state.show_rename_dialog && !rename_target) {
      rename_target = name;
    }

    if (context_state.show_change_type_dialog && !change_type_target) {
      change_type_target = name;
    }

    ImGui::TableNextColumn();
    if (value.IsString()) {
      if (const auto updated = StringPropertyWidget(value)) {
        dispatcher.enqueue<UpdatePropertyEvent>(name, *updated);
      }
    }
    else if (value.IsInt()) {
      if (const auto updated = IntPropertyWidget(value)) {
        dispatcher.enqueue<UpdatePropertyEvent>(name, *updated);
      }
    }
    else if (value.IsFloat()) {
      if (const auto updated = FloatPropertyWidget(value)) {
        dispatcher.enqueue<UpdatePropertyEvent>(name, *updated);
      }
    }
    else if (value.IsBool()) {
      if (const auto updated = BoolPropertyWidget(value)) {
        dispatcher.enqueue<UpdatePropertyEvent>(name, *updated);
      }
    }
    else if (value.IsColor()) {
      if (const auto updated = ColorPropertyWidget(value)) {
        dispatcher.enqueue<UpdatePropertyEvent>(name, *updated);
      }
    }
    else if (value.IsObject()) {
      if (const auto updated = ObjectPropertyWidget(value)) {
        dispatcher.enqueue<UpdatePropertyEvent>(name, *updated);
      }
    }
    else if (value.IsFile()) {
      if (const auto updated = FilePropertyWidget(value)) {
        dispatcher.enqueue<UpdatePropertyEvent>(name, *updated);
      }
    }
  }
}

}  // namespace

void UpdatePropertyTable(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  constexpr auto flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
                         ImGuiTableFlags_ScrollY | ImGuiTableFlags_PadOuterX;

  const auto& current = registry.ctx<ActivePropertyContext>();
  const auto& context = Sys::GetCurrentContext(registry);

  if (ImGui::BeginTable("##PropertyTable", 2, flags)) {
    if (current.entity == entt::null) {
      // TODO native map properties
    }
    else {
      if (const auto* tileset = registry.try_get<Tileset>(current.entity)) {
        ShowNativeTilesetProperties(*tileset, dispatcher);
      }
      else if (const auto* layer = registry.try_get<Layer>(current.entity)) {
        ShowNativeLayerProperties(*layer, dispatcher);
      }
      else if (const auto* object = registry.try_get<Object>(current.entity)) {
        ShowNativeObjectProperties(*object, dispatcher);
      }
    }

    bool isItemContextOpen = false;
    ShowCustomProperties(registry, dispatcher, context, isItemContextOpen);

    if (!isItemContextOpen && ImGui::BeginPopupContextWindow("##PropertyTableContext")) {
      context_state.show_add_dialog =
          ImGui::MenuItem(TAC_ICON_ADD " Add New Property...");

      ImGui::EndPopup();
    }

    ImGui::EndTable();
  }

  if (context_state.show_add_dialog) {
    OpenAddPropertyDialog();
    context_state.show_add_dialog = false;
  }

  if (context_state.show_rename_dialog) {
    OpenRenamePropertyDialog(rename_target.value());
    rename_target.reset();
    context_state.show_rename_dialog = false;
  }

  if (context_state.show_change_type_dialog) {
    const auto& name = change_type_target.value();
    const auto type = Sys::GetPropertyType(registry, context.id, name);
    OpenChangePropertyTypeDialog(name, type);
    change_type_target.reset();
    context_state.show_change_type_dialog = false;
  }
}

}  // namespace Tactile

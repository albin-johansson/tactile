#include "property_table.hpp"

#include <locale>  // locale, isalpha, isdigit, isspace
#include <string>  // string

#include <tactile_def.hpp>

#include <imgui.h>

#include "core/components/layer.hpp"
#include "core/components/object.hpp"
#include "core/components/property.hpp"
#include "core/components/tileset.hpp"
#include "core/map.hpp"
#include "core/systems/property_system.hpp"
#include "editor/events/layer_events.hpp"
#include "editor/events/object_events.hpp"
#include "editor/events/property_events.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/properties/dialogs/add_property_dialog.hpp"
#include "editor/gui/properties/dialogs/change_property_type_dialog.hpp"
#include "editor/gui/properties/dialogs/rename_property_dialog.hpp"
#include "editor/gui/properties/items/bool_property_widget.hpp"
#include "editor/gui/properties/items/color_property_widget.hpp"
#include "editor/gui/properties/items/file_property_widget.hpp"
#include "editor/gui/properties/items/float_property_widget.hpp"
#include "editor/gui/properties/items/int_property_widget.hpp"
#include "editor/gui/properties/items/object_property_widget.hpp"
#include "editor/gui/properties/items/property_item_context_menu.hpp"
#include "editor/gui/properties/items/string_property_widget.hpp"
#include "editor/gui/scoped.hpp"

namespace Tactile {
namespace {

constinit PropertyItemContextMenuState gContextState;
inline Maybe<std::string> gRenameTarget;
inline Maybe<std::string> gChangeTypeTarget;

void PrepareTableRow(const CStr label)
{
  ImGui::TableNextRow();
  ImGui::TableNextColumn();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(label);
}

[[nodiscard]] auto NativeNameRow(const std::string& name,
                                 const bool validateAsFileName = false)
    -> Maybe<std::string>
{
  const Scoped::ID scope{"##NativeNameRow"};

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

    return StringPropertyWidget(name, flags, filter);
  }
  else {
    return StringPropertyWidget(name, flags);
  }
}

[[nodiscard]] auto NativeOpacityRow(const float opacity) -> Maybe<float>
{
  const Scoped::ID scope{"##NativeOpacityRow"};
  PrepareTableRow("Opacity");

  ImGui::TableNextColumn();
  return FloatPropertyWidget(opacity, 0.0f, 1.0f);
}

[[nodiscard]] auto NativeVisibilityRow(const bool visible) -> Maybe<bool>
{
  const Scoped::ID scope{"##NativeVisibilityRow"};
  PrepareTableRow("Visible");

  ImGui::TableNextColumn();
  return BoolPropertyWidget(visible);
}

void NativeReadOnlyRow(const CStr label, const CStr value)
{
  PrepareTableRow(label);

  ImGui::TableNextColumn();
  ImGui::TextUnformatted(value);
}

void NativeReadOnlyRow(const CStr label, const float value)
{
  PrepareTableRow(label);

  ImGui::TableNextColumn();
  ImGui::Text("%.2f", value);
}

void NativeReadOnlyRow(const CStr label, const int32 value)
{
  PrepareTableRow(label);

  ImGui::TableNextColumn();
  ImGui::Text("%d", value);
}

void NativeReadOnlyRow(const CStr label, const usize value)
{
  PrepareTableRow(label);

  ImGui::TableNextColumn();
  ImGui::Text("%llu", static_cast<ulonglong>(value)); /* Cast to avoid format warnings */
}

void ShowNativeMapProperties(const std::string& name, const Map& map)
{
  NativeReadOnlyRow("Type", "Map");
  NativeReadOnlyRow("Name", name.c_str());

  NativeReadOnlyRow("Tile width", map.tile_width);
  NativeReadOnlyRow("Tile height", map.tile_height);

  NativeReadOnlyRow("Row count", map.row_count);
  NativeReadOnlyRow("Column count", map.column_count);

  ImGui::Separator();
}

void ShowNativeTilesetProperties(const std::string& name,
                                 const Tileset& tileset,
                                 entt::dispatcher& dispatcher)
{
  NativeReadOnlyRow("Type", "Tileset");

  if constexpr (cen::is_debug_build()) {
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

  ImGui::Separator();
}

void ShowNativeLayerProperties(const Layer& layer, entt::dispatcher& dispatcher)
{
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
    NativeReadOnlyRow("ID", layer.id);
  }

  if (const auto value = NativeOpacityRow(layer.opacity)) {
    dispatcher.enqueue<SetLayerOpacityEvent>(layer.id, *value);
  }

  if (const auto value = NativeVisibilityRow(layer.visible)) {
    dispatcher.enqueue<SetLayerVisibleEvent>(layer.id, *value);
  }

  ImGui::Separator();
}

void ShowNativeObjectProperties(const std::string& name,
                                const Object& object,
                                entt::dispatcher& dispatcher)
{
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

  if constexpr (cen::is_debug_build()) {
    NativeReadOnlyRow("ID", object.id);
  }

  if (const auto updatedName = NativeNameRow(name)) {
    dispatcher.enqueue<SetObjectNameEvent>(object.id, *updatedName);
  }

  NativeReadOnlyRow("X", object.x);
  NativeReadOnlyRow("Y", object.y);

  if (object.type != ObjectType::Point) {
    NativeReadOnlyRow("Width", object.width);
    NativeReadOnlyRow("Height", object.height);
  }

  if (const auto visible = NativeVisibilityRow(object.visible)) {
    dispatcher.enqueue<SetObjectVisibilityEvent>(object.id, *visible);
  }

  PrepareTableRow("Tag");

  ImGui::TableNextColumn();
  if (const auto tag = StringPropertyWidget(object.tag)) {
    dispatcher.enqueue<SetObjectTagEvent>(object.id, *tag);
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

    const Scoped::ID scope{name.c_str()};

    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::AlignTextToFramePadding();
    ImGui::Selectable(name.c_str());

    if (!isItemContextOpen) {
      isItemContextOpen = PropertyItemContextMenu(dispatcher, name, gContextState);
    }

    if (gContextState.show_rename_dialog && !gRenameTarget) {
      gRenameTarget = name;
    }

    if (gContextState.show_change_type_dialog && !gChangeTypeTarget) {
      gChangeTypeTarget = name;
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

  if (Scoped::Table table{"##PropertyTable", 2, flags}; table.IsOpen()) {
    if (current.entity == entt::null) {
      ShowNativeMapProperties(context.name, registry.ctx<Map>());
    }
    else {
      if (const auto* tileset = registry.try_get<Tileset>(current.entity)) {
        ShowNativeTilesetProperties(context.name, *tileset, dispatcher);
      }
      else if (const auto* layer = registry.try_get<Layer>(current.entity)) {
        ShowNativeLayerProperties(*layer, dispatcher);
      }
      else if (const auto* object = registry.try_get<Object>(current.entity)) {
        ShowNativeObjectProperties(context.name, *object, dispatcher);
      }
    }

    bool isItemContextOpen = false;
    ShowCustomProperties(registry, dispatcher, context, isItemContextOpen);

    if (!isItemContextOpen) {
      if (auto popup = Scoped::Popup::ForWindow("##PropertyTableContext");
          popup.IsOpen()) {
        gContextState.show_add_dialog =
            ImGui::MenuItem(TAC_ICON_ADD " Add New Property...");
      }
    }
  }

  if (gContextState.show_add_dialog) {
    OpenAddPropertyDialog();
    gContextState.show_add_dialog = false;
  }

  if (gContextState.show_rename_dialog) {
    OpenRenamePropertyDialog(gRenameTarget.value());
    gRenameTarget.reset();
    gContextState.show_rename_dialog = false;
  }

  if (gContextState.show_change_type_dialog) {
    const auto& name = gChangeTypeTarget.value();
    const auto type = Sys::GetProperty(registry, context, name).value.GetType().value();
    OpenChangePropertyTypeDialog(name, type);
    gChangeTypeTarget.reset();
    gContextState.show_change_type_dialog = false;
  }
}

}  // namespace Tactile

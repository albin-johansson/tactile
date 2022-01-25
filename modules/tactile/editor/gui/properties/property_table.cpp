#include "property_table.hpp"

#include <locale>  // locale, isalpha, isdigit, isspace

#include <imgui.h>
#include <tactile_def.hpp>

#include "build.hpp"
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
#include "editor/gui/common/input_widgets.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"

namespace tactile {
namespace {

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

    return InputString("##NativeNameRowInput", name, nullptr, flags, filter);
  }
  else {
    return InputString("##NativeNameRowInput", name, nullptr, flags);
  }
}

[[nodiscard]] auto NativeOpacityRow(const float opacity) -> Maybe<float>
{
  PrepareTableRow("Opacity");

  ImGui::TableNextColumn();
  return InputWidget("##NativeOpacityRow", opacity, 0.0f, 1.0f);
}

[[nodiscard]] auto NativeVisibilityRow(const bool visible) -> Maybe<bool>
{
  PrepareTableRow("Visible");

  ImGui::TableNextColumn();
  return InputWidget("##NativeVisibilityRow", visible);
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
                                 const Tileset& tileset,
                                 entt::dispatcher& dispatcher)
{
  NativeReadOnlyRow("Type", "Tileset");

  if constexpr (IsDebugBuild()) {
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

  if constexpr (IsDebugBuild()) {
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

  if constexpr (IsDebugBuild()) {
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
  if (const auto tag = InputString("##NativeObjectTagInput", object.tag)) {
    dispatcher.enqueue<SetObjectTagEvent>(object.id, *tag);
  }
}

}  // namespace

void PropertyTable::Update(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  constexpr auto flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
                         ImGuiTableFlags_ScrollY | ImGuiTableFlags_PadOuterX;

  const auto& current = registry.ctx<active_attribute_context>();
  const auto& context = sys::GetCurrentContext(registry);

  if (scoped::Table table{"##PropertyTable", 2, flags}; table.IsOpen()) {
    if (current.entity == entt::null) {
      ShowNativeMapProperties(context.name, registry.ctx<MapInfo>());
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
      if (auto popup = scoped::Popup::ForWindow("##PropertyTableContext");
          popup.IsOpen()) {
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
    const auto type = sys::GetProperty(registry, context, name).value.type();
    dispatcher.enqueue<ShowChangePropertyTypeDialogEvent>(name, type);
    mChangeTypeTarget.reset();
    mContextState.show_change_type_dialog = false;
  }
}

void PropertyTable::ShowCustomProperties(const entt::registry& registry,
                                         entt::dispatcher& dispatcher,
                                         const attribute_context& context,
                                         bool& isItemContextOpen)
{
  bool first = true;

  for (const auto entity : context.properties) {
    const auto& property = registry.get<Property>(entity);

    const auto& name = property.name;
    const auto& value = property.value;

    const scoped::ID scope{name.c_str()};

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
      if (const auto updated = InputString("##CustomPropertyInput", value.as_string())) {
        dispatcher.enqueue<UpdatePropertyEvent>(name, *updated);
      }
    }
    else if (value.is_int()) {
      if (const auto updated = InputWidget("##CustomPropertyInput", value.as_int())) {
        dispatcher.enqueue<UpdatePropertyEvent>(name, *updated);
      }
    }
    else if (value.is_float()) {
      if (const auto updated = InputWidget("##CustomPropertyInput", value.as_float())) {
        dispatcher.enqueue<UpdatePropertyEvent>(name, *updated);
      }
    }
    else if (value.is_bool()) {
      if (const auto updated = InputWidget("##CustomPropertyInput", value.as_bool())) {
        dispatcher.enqueue<UpdatePropertyEvent>(name, *updated);
      }
    }
    else if (value.is_color()) {
      if (const auto updated = InputWidget("##CustomPropertyInput", value.as_color())) {
        dispatcher.enqueue<UpdatePropertyEvent>(name, *updated);
      }
    }
    else if (value.is_object()) {
      if (const auto updated = InputWidget("##CustomPropertyInput", value.as_object())) {
        dispatcher.enqueue<UpdatePropertyEvent>(name, *updated);
      }
    }
    else if (value.is_file()) {
      if (const auto updated = InputFile("##CustomPropertyInput", value.as_file())) {
        dispatcher.enqueue<UpdatePropertyEvent>(name, *updated);
      }
    }

    first = false;
  }
}

}  // namespace tactile

#include "property_table.hpp"

#include <imgui.h>

#include <string>  // string

#include "aliases/maybe.hpp"
#include "core/components/property.hpp"
#include "core/systems/property_system.hpp"
#include "dialogs/add_property_dialog.hpp"
#include "dialogs/change_property_type_dialog.hpp"
#include "dialogs/rename_property_dialog.hpp"
#include "events/property_events.hpp"
#include "gui/icons.hpp"
#include "items/bool_property_widget.hpp"
#include "items/color_property_widget.hpp"
#include "items/file_property_widget.hpp"
#include "items/float_property_widget.hpp"
#include "items/int_property_widget.hpp"
#include "items/object_property_widget.hpp"
#include "items/property_item_context_menu.hpp"
#include "items/string_property_widget.hpp"
#include "utils/scope_id.hpp"

namespace Tactile {
namespace {

constinit PropertyItemContextMenuState context_state;
inline Maybe<std::string> rename_target;
inline Maybe<std::string> change_type_target;

void StringValue(const std::string& name,
                 const PropertyValue& property,
                 entt::dispatcher& dispatcher)
{
  if (const auto str = StringPropertyWidget(property)) {
    dispatcher.enqueue<UpdatePropertyEvent>(name, *str);
  }
}

void IntValue(const std::string& name,
              const PropertyValue& property,
              entt::dispatcher& dispatcher)
{
  if (const auto value = IntPropertyWidget(property)) {
    dispatcher.enqueue<UpdatePropertyEvent>(name, *value);
  }
}

void FloatValue(const std::string& name,
                const PropertyValue& property,
                entt::dispatcher& dispatcher)
{
  if (const auto value = FloatPropertyWidget(property)) {
    dispatcher.enqueue<UpdatePropertyEvent>(name, *value);
  }
}

void BoolValue(const std::string& name,
               const PropertyValue& property,
               entt::dispatcher& dispatcher)
{
  if (const auto value = BoolPropertyWidget(property)) {
    dispatcher.enqueue<UpdatePropertyEvent>(name, *value);
  }
}

void ColorValue(const std::string& name,
                const PropertyValue& property,
                entt::dispatcher& dispatcher)
{
  if (const auto color = ColorPropertyWidget(property)) {
    dispatcher.enqueue<UpdatePropertyEvent>(name, *color);
  }
}

void ObjectValue(const std::string& name,
                 const PropertyValue& property,
                 entt::dispatcher& dispatcher)
{
  if (const auto value = ObjectPropertyWidget(property)) {
    dispatcher.enqueue<UpdatePropertyEvent>(name, *value);
  }
}

void FileValue(const std::string& name,
               const PropertyValue& property,
               entt::dispatcher& dispatcher)
{
  if (const auto path = FilePropertyWidget(property)) {
    dispatcher.enqueue<UpdatePropertyEvent>(name, *path);
  }
}

}  // namespace

void UpdatePropertyTable(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  constexpr auto flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
                         ImGuiTableFlags_ScrollY | ImGuiTableFlags_PadOuterX;

  if (ImGui::BeginTable("##PropertyTable", 2, flags)) {
    const auto& context = Sys::GetCurrentContext(registry);
    for (const auto entity : context.properties) {
      const auto& property = registry.get<Property>(entity);
      const auto& name = property.name;
      const auto& value = property.value;

      const ScopeID uid{name.c_str()};

      ImGui::TableNextRow();
      ImGui::TableNextColumn();

      ImGui::AlignTextToFramePadding();
      ImGui::Selectable(name.c_str());

      PropertyItemContextMenu(dispatcher, name, context_state);

      if (context_state.show_rename_dialog && !rename_target) {
        rename_target = name;
      }

      if (context_state.show_change_type_dialog && !change_type_target) {
        change_type_target = name;
      }

      ImGui::TableNextColumn();
      if (value.IsString()) {
        StringValue(name, value, dispatcher);
      }
      else if (value.IsInt()) {
        IntValue(name, value, dispatcher);
      }
      else if (value.IsFloat()) {
        FloatValue(name, value, dispatcher);
      }
      else if (value.IsBool()) {
        BoolValue(name, value, dispatcher);
      }
      else if (value.IsColor()) {
        ColorValue(name, value, dispatcher);
      }
      else if (value.IsObject()) {
        ObjectValue(name, value, dispatcher);
      }
      else if (value.IsFile()) {
        FileValue(name, value, dispatcher);
      }
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
    OpenChangePropertyTypeDialog(change_type_target.value());
    change_type_target.reset();
    context_state.show_change_type_dialog = false;
  }
}

}  // namespace Tactile

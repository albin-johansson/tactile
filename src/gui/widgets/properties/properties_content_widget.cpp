#include "properties_content_widget.hpp"

#include <imgui.h>

#include <string>  // string

#include "aliases/maybe.hpp"
#include "bool_property_widget.hpp"
#include "color_property_widget.hpp"
#include "core/components/property.hpp"
#include "core/systems/property_system.hpp"
#include "dialogs/add_property_dialog.hpp"
#include "dialogs/change_property_type_dialog.hpp"
#include "dialogs/rename_property_dialog.hpp"
#include "events/property_events.hpp"
#include "file_property_widget.hpp"
#include "float_property_widget.hpp"
#include "gui/icons.hpp"
#include "int_property_widget.hpp"
#include "object_property_widget.hpp"
#include "string_property_widget.hpp"
#include "utils/scope_id.hpp"

namespace Tactile {
namespace {

constinit bool show_add_dialog = false;
constinit bool show_rename_dialog = false;
constinit bool show_change_type_dialog = false;
inline Maybe<std::string> rename_target;
inline Maybe<std::string> change_type_target;

void StringValue(const std::string_view name,
                 const PropertyValue& property,
                 entt::dispatcher& dispatcher)
{
  if (const auto str = StringPropertyWidget(property))
  {
    dispatcher.enqueue<SetPropertyValueEvent>(name, *str);
  }
}

void IntValue(const std::string_view name,
              const PropertyValue& property,
              entt::dispatcher& dispatcher)
{
  if (const auto value = IntPropertyWidget(property))
  {
    dispatcher.enqueue<SetPropertyValueEvent>(name, *value);
  }
}

void FloatValue(const std::string_view name,
                const PropertyValue& property,
                entt::dispatcher& dispatcher)
{
  if (const auto value = FloatPropertyWidget(property))
  {
    dispatcher.enqueue<SetPropertyValueEvent>(name, *value);
  }
}

void BoolValue(const std::string_view name,
               const PropertyValue& property,
               entt::dispatcher& dispatcher)
{
  if (const auto value = BoolPropertyWidget(property))
  {
    dispatcher.enqueue<SetPropertyValueEvent>(name, *value);
  }
}

void ColorValue(const std::string_view name,
                const PropertyValue& property,
                entt::dispatcher& dispatcher)
{
  if (const auto color = ColorPropertyWidget(property))
  {
    dispatcher.enqueue<SetPropertyValueEvent>(name, *color);
  }
}

void ObjectValue(const std::string_view name,
                 const PropertyValue& property,
                 entt::dispatcher& dispatcher)
{
  if (const auto value = ObjectPropertyWidget(property))
  {
    dispatcher.enqueue<SetPropertyValueEvent>(name, *value);
  }
}

void FileValue(const std::string_view name,
               const PropertyValue& property,
               entt::dispatcher& dispatcher)
{
  if (const auto path = FilePropertyWidget(property))
  {
    dispatcher.enqueue<SetPropertyValueEvent>(name, *path);
  }
}

void PropertyItemContextMenu(entt::dispatcher& dispatcher, const std::string& name)
{
  if (ImGui::BeginPopupContextItem("##PropertyItemContext"))
  {
    show_add_dialog = ImGui::MenuItem(TAC_ICON_ADD " Add new property...");
    ImGui::Separator();

    show_rename_dialog = ImGui::MenuItem(TAC_ICON_EDIT " Rename property...");
    if (show_rename_dialog)
    {
      rename_target = name;
    }

    show_change_type_dialog = ImGui::MenuItem(ICON_FA_SHAPES " Change type...");
    if (show_change_type_dialog)
    {
      change_type_target = name;
    }
    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove property"))
    {
      dispatcher.enqueue<RemovePropertyEvent>(name);
    }

    ImGui::EndPopup();
  }
}

}  // namespace

void UpdatePropertiesContentWidget(const entt::registry& registry,
                                   entt::dispatcher& dispatcher)
{
  constexpr auto flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders |
                         ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY;

  if (ImGui::BeginTable("PropertiesTable", 2, flags))
  {
    const auto& context = Sys::GetCurrentContext(registry);
    for (const auto entity : context.properties)
    {
      const auto& property = registry.get<Property>(entity);
      const auto& name = property.name;
      const auto& value = property.value;

      const ScopeID uid{name.c_str()};

      ImGui::TableNextRow();
      ImGui::TableNextColumn();

      ImGui::AlignTextToFramePadding();
      ImGui::Selectable(name.c_str());

      PropertyItemContextMenu(dispatcher, name);

      ImGui::TableNextColumn();
      if (value.IsString())
      {
        StringValue(name, value, dispatcher);
      }
      else if (value.IsInt())
      {
        IntValue(name, value, dispatcher);
      }
      else if (value.IsFloat())
      {
        FloatValue(name, value, dispatcher);
      }
      else if (value.IsBool())
      {
        BoolValue(name, value, dispatcher);
      }
      else if (value.IsColor())
      {
        ColorValue(name, value, dispatcher);
      }
      else if (value.IsObject())
      {
        ObjectValue(name, value, dispatcher);
      }
      else if (value.IsFile())
      {
        FileValue(name, value, dispatcher);
      }
    }

    ImGui::EndTable();
  }

  if (show_add_dialog)
  {
    OpenAddPropertyDialog();
    show_add_dialog = false;
  }

  if (show_rename_dialog)
  {
    OpenRenamePropertyDialog(rename_target.value());
    rename_target.reset();
    show_rename_dialog = false;
  }

  if (show_change_type_dialog)
  {
    OpenChangePropertyTypeDialog(change_type_target.value());
    change_type_target.reset();
    show_change_type_dialog = false;
  }
}

}  // namespace Tactile

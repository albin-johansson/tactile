#include "properties_content_widget.hpp"

#include <imgui.h>

#include <string>  // string

#include "aliases/maybe.hpp"
#include "core/map_document.hpp"
#include "events/properties/remove_property_event.hpp"
#include "events/properties/rename_property_event.hpp"
#include "events/properties/set_property_value_event.hpp"
#include "gui/icons.hpp"
#include "gui/widgets/properties/bool_property_widget.hpp"
#include "gui/widgets/properties/color_property_widget.hpp"
#include "gui/widgets/properties/dialogs/add_property_dialog.hpp"
#include "gui/widgets/properties/dialogs/change_property_type_dialog.hpp"
#include "gui/widgets/properties/dialogs/rename_property_dialog.hpp"
#include "gui/widgets/properties/file_property_widget.hpp"
#include "gui/widgets/properties/float_property_widget.hpp"
#include "gui/widgets/properties/int_property_widget.hpp"
#include "gui/widgets/properties/object_property_widget.hpp"
#include "gui/widgets/properties/string_property_widget.hpp"
#include "utils/scope_id.hpp"

namespace Tactile {
namespace {

inline bool show_add_dialog = false;
inline bool show_rename_dialog = false;
inline bool show_change_type_dialog = false;
inline Maybe<std::string> rename_target;
inline Maybe<std::string> change_type_target;

void StringValue(const std::string_view name,
                 const Property& property,
                 entt::dispatcher& dispatcher)
{
  if (const auto str = StringPropertyWidget(property))
  {
    dispatcher.enqueue<SetPropertyValueEvent>(name, *str);
  }
}

void IntValue(const std::string_view name,
              const Property& property,
              entt::dispatcher& dispatcher)
{
  if (const auto value = IntPropertyWidget(property))
  {
    dispatcher.enqueue<SetPropertyValueEvent>(name, *value);
  }
}

void FloatValue(const std::string_view name,
                const Property& property,
                entt::dispatcher& dispatcher)
{
  if (const auto value = FloatPropertyWidget(property))
  {
    dispatcher.enqueue<SetPropertyValueEvent>(name, *value);
  }
}

void BoolValue(const std::string_view name,
               const Property& property,
               entt::dispatcher& dispatcher)
{
  if (const auto value = BoolPropertyWidget(property))
  {
    dispatcher.enqueue<SetPropertyValueEvent>(name, *value);
  }
}

void ColorValue(const std::string_view name,
                const Property& property,
                entt::dispatcher& dispatcher)
{
  if (const auto color = ColorPropertyWidget(property))
  {
    dispatcher.enqueue<SetPropertyValueEvent>(name, *color);
  }
}

void ObjectValue(const std::string_view name,
                 const Property& property,
                 entt::dispatcher& dispatcher)
{
  if (const auto value = ObjectPropertyWidget(property))
  {
    dispatcher.enqueue<SetPropertyValueEvent>(name, *value);
  }
}

void FileValue(const std::string_view name,
               const Property& property,
               entt::dispatcher& dispatcher)
{
  if (const auto path = FilePropertyWidget(property))
  {
    dispatcher.enqueue<SetPropertyValueEvent>(name, *path);
  }
}

}  // namespace

void UpdatePropertiesContentWidget(const MapDocument& document,
                                   entt::dispatcher& dispatcher)
{
  constexpr auto flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders |
                         ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY;

  if (ImGui::BeginTable("PropertiesTable", 2, flags))
  {
    for (const auto& [name, property] : document.GetProperties())
    {
      const ScopeID uid{name.c_str()};

      ImGui::TableNextRow();
      ImGui::TableNextColumn();

      ImGui::AlignTextToFramePadding();
      ImGui::Selectable(name.c_str());

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

      ImGui::TableNextColumn();
      if (property.IsString())
      {
        StringValue(name, property, dispatcher);
      }
      else if (property.IsInteger())
      {
        IntValue(name, property, dispatcher);
      }
      else if (property.IsFloating())
      {
        FloatValue(name, property, dispatcher);
      }
      else if (property.IsBoolean())
      {
        BoolValue(name, property, dispatcher);
      }
      else if (property.IsColor())
      {
        ColorValue(name, property, dispatcher);
      }
      else if (property.IsObject())
      {
        ObjectValue(name, property, dispatcher);
      }
      else if (property.IsFile())
      {
        FileValue(name, property, dispatcher);
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

#include "properties_widget.hpp"

#include <IconsFontAwesome5.h>

#include <string_view>  // string_view

#include "aliases/ints.hpp"
#include "core/events/set_property_value_event.hpp"
#include "core/model.hpp"
#include "gui/widgets/button_ex.hpp"
#include "gui/widgets/help_marker.hpp"
#include "gui/widgets/properties/bool_property_widget.hpp"
#include "gui/widgets/properties/color_property_widget.hpp"
#include "gui/widgets/properties/file_property_widget.hpp"
#include "gui/widgets/properties/float_property_widget.hpp"
#include "gui/widgets/properties/int_property_widget.hpp"
#include "gui/widgets/properties/object_property_widget.hpp"
#include "gui/widgets/properties/string_property_widget.hpp"
#include "imgui.h"

namespace Tactile {
namespace {

inline bool is_visible = true;

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

void ShowProperties(const MapDocument& document, entt::dispatcher& dispatcher)
{
  constexpr auto flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders |
                         ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY;

  if (ImGui::BeginTable("PropertiesTable", 2, flags))
  {
    for (const auto& [name, property] : document.GetProperties())
    {
      ImGui::TableNextRow();

      ImGui::TableNextColumn();
      ImGui::AlignTextToFramePadding();
      ImGui::TextUnformatted(name.c_str());

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
}

}  // namespace

void UpdatePropertiesWidget(const Model& model, entt::dispatcher& dispatcher)
{
  if (!is_visible || !model.GetActiveMapId())
  {
    return;
  }

  constexpr auto flags =
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

  if (ImGui::Begin("Properties", &is_visible, flags))
  {
    if (ButtonEx(ICON_FA_PLUS_CIRCLE, "Add property."))
    {}

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_MINUS_CIRCLE, "Remove property."))
    {}

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_EDIT, "Rename property."))
    {}

    const auto* document = model.GetActiveDocument();

    ImGui::Separator();
    ImGui::Text("Context: %s", document->GetName().data());
    ImGui::SameLine();
    HelpMarker("The owner of the shown properties.");
    ImGui::Separator();

    if (document->GetPropertyCount() != 0)
    {
      ShowProperties(*document, dispatcher);
    }
    else
    {
      ImGui::Text("No available properties.");
    }
  }

  ImGui::End();
}

void SetPropertiesWidgetVisible(const bool visible) noexcept
{
  is_visible = visible;
}

auto IsPropertiesWidgetVisible() noexcept -> bool
{
  return is_visible;
}

}  // namespace Tactile

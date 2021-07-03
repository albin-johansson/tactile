#include "properties_content_widget.hpp"

#include <imgui.h>

#include "core/events/set_property_value_event.hpp"
#include "core/map_document.hpp"
#include "gui/widgets/properties/bool_property_widget.hpp"
#include "gui/widgets/properties/color_property_widget.hpp"
#include "gui/widgets/properties/file_property_widget.hpp"
#include "gui/widgets/properties/float_property_widget.hpp"
#include "gui/widgets/properties/int_property_widget.hpp"
#include "gui/widgets/properties/object_property_widget.hpp"
#include "gui/widgets/properties/string_property_widget.hpp"

namespace Tactile {
namespace {

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

}  // namespace Tactile

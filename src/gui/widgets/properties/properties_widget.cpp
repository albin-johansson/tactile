#include "properties_widget.hpp"

#include <IconsFontAwesome5.h>

#include "core/model.hpp"
#include "gui/widgets/button_ex.hpp"
#include "imgui.h"

namespace tactile {
namespace {

inline bool is_visible = true;

inline constexpr auto table_flags =
    ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable;

}  // namespace

void UpdatePropertiesWidget(const Model& model, entt::dispatcher& dispatcher)
{
  if (!is_visible || !model.GetActiveMapId())
  {
    return;
  }

  if (ImGui::Begin("Properties", &is_visible, ImGuiWindowFlags_NoCollapse))
  {
    if (ButtonEx(ICON_FA_PLUS_CIRCLE, "Add property."))
    {}

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_MINUS_CIRCLE, "Remove property."))
    {}

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_EDIT, "Rename property."))
    {}

    ImGui::Separator();

    if (ImGui::BeginTable("PropertiesTable", 2, table_flags))
    {
      ImGui::TableSetupColumn("Name");
      ImGui::TableSetupColumn("Value");
      ImGui::TableHeadersRow();

      for (int i = 0; i < 4; ++i)
      {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Foo");

        ImGui::TableSetColumnIndex(1);
        ImGui::Text("Bar");
      }

      ImGui::EndTable();
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

}  // namespace tactile

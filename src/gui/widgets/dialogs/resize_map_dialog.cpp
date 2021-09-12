#include "resize_map_dialog.hpp"

#include <imgui.h>

#include "events/map_events.hpp"
#include "gui/widgets/alignment.hpp"
#include "gui/widgets/common/button.hpp"

namespace Tactile {
namespace {

constexpr auto flags =
    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

constinit int row_count = 0;
constinit int col_count = 0;

[[nodiscard]] auto is_input_valid() noexcept -> bool
{
  return row_count > 0 && col_count > 0;
}

}  // namespace

void UpdateResizeMapDialog(entt::dispatcher& dispatcher)
{
  CenterNextWindowOnAppearance();
  if (ImGui::BeginPopupModal("Resize Map", nullptr, flags))
  {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Rows:     ");
    ImGui::SameLine();
    ImGui::InputInt("##RowCountInput", &row_count);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Columns:  ");
    ImGui::SameLine();
    ImGui::InputInt("##ColumnCountInput", &col_count);

    ImGui::Spacing();
    if (Button("OK", nullptr, is_input_valid()))
    {
      dispatcher.enqueue<ResizeMapEvent>(row_count, col_count);
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (Button("Cancel"))
    {
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
}

void OpenResizeMapDialog(const int currentRows, const int currentColumns)
{
  row_count = currentRows;
  col_count = currentColumns;
  ImGui::OpenPopup("Resize Map");
}

}  // namespace Tactile

#include "resize_map_dialog.hpp"

#include <imgui.h>

#include "tactile/editor/events/map_events.hpp"
#include "tactile/editor/gui/widgets/alignment.hpp"
#include "tactile/editor/gui/widgets/common/button.hpp"
#include "tactile/editor/gui/widgets/common/modal.hpp"

namespace Tactile {
namespace {

constexpr auto flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

constinit int row_count = 0;
constinit int col_count = 0;

[[nodiscard]] auto IsInputValid() noexcept -> bool
{
  return row_count > 0 && col_count > 0;
}

}  // namespace

void UpdateResizeMapDialog(entt::dispatcher& dispatcher)
{
  CenterNextWindowOnAppearance();
  if (auto modal = Modal{"Resize Map", flags}) {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Rows:     ");
    ImGui::SameLine();
    ImGui::DragInt("##RowCountInput", &row_count, 1.0f, 1, 10'000);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Columns:  ");
    ImGui::SameLine();
    ImGui::DragInt("##ColumnCountInput", &col_count, 1.0f, 1, 10'000);

    ImGui::Spacing();
    if (Button("OK", nullptr, IsInputValid())) {
      dispatcher.enqueue<ResizeMapEvent>(row_count, col_count);
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (Button("Cancel")) {
      ImGui::CloseCurrentPopup();
    }
  }
}

void OpenResizeMapDialog(const int currentRows, const int currentColumns)
{
  row_count = currentRows;
  col_count = currentColumns;
  ImGui::OpenPopup("Resize Map");
}

}  // namespace Tactile

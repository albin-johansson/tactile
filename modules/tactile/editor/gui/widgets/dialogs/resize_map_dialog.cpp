#include "resize_map_dialog.hpp"

#include <imgui.h>

#include "editor/events/map_events.hpp"
#include "editor/gui/widgets/alignment.hpp"
#include "editor/gui/widgets/common/button.hpp"
#include "editor/gui/widgets/common/modal.hpp"

namespace Tactile {
namespace {

constexpr auto flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

constinit usize row_count = 0;
constinit usize col_count = 0;

[[nodiscard]] auto IsInputValid() noexcept -> bool
{
  return row_count > 0u && col_count > 0u;
}

}  // namespace

void UpdateResizeMapDialog(entt::dispatcher& dispatcher)
{
  CenterNextWindowOnAppearance();
  if (auto modal = Modal{"Resize Map", flags}) {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Rows:     ");
    ImGui::SameLine();

    auto rows = static_cast<int>(row_count);
    ImGui::DragInt("##RowCountInput", &rows, 1.0f, 1, 10'000);
    row_count = static_cast<usize>(rows);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Columns:  ");
    ImGui::SameLine();

    auto cols = static_cast<int>(col_count);
    ImGui::DragInt("##ColumnCountInput", &cols, 1.0f, 1, 10'000);
    col_count = static_cast<usize>(cols);

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

void OpenResizeMapDialog(const usize currentRows, const usize currentColumns)
{
  row_count = currentRows;
  col_count = currentColumns;
  ImGui::OpenPopup("Resize Map");
}

}  // namespace Tactile

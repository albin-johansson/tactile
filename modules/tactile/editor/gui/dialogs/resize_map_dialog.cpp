#include "resize_map_dialog.hpp"

#include <imgui.h>

#include "editor/events/map_events.hpp"
#include "editor/gui/alignment.hpp"
#include "editor/gui/common/button.hpp"
#include "editor/gui/scoped.hpp"

namespace Tactile {
namespace {

constexpr auto gFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

constinit usize gRowCount = 0;
constinit usize gColCount = 0;

[[nodiscard]] auto IsInputValid() noexcept -> bool
{
  return gRowCount > 0u && gColCount > 0u;
}

}  // namespace

void UpdateResizeMapDialog(entt::dispatcher& dispatcher)
{
  CenterNextWindowOnAppearance();
  if (Scoped::Modal modal{"Resize Map", gFlags}; modal.IsOpen()) {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Rows:     ");
    ImGui::SameLine();

    auto rows = static_cast<int>(gRowCount);
    ImGui::DragInt("##RowCountInput", &rows, 1.0f, 1, 10'000);
    gRowCount = static_cast<usize>(rows);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Columns:  ");
    ImGui::SameLine();

    auto cols = static_cast<int>(gColCount);
    ImGui::DragInt("##ColumnCountInput", &cols, 1.0f, 1, 10'000);
    gColCount = static_cast<usize>(cols);

    ImGui::Spacing();
    if (Button("OK", nullptr, IsInputValid())) {
      dispatcher.enqueue<ResizeMapEvent>(gRowCount, gColCount);
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
  gRowCount = currentRows;
  gColCount = currentColumns;
  ImGui::OpenPopup("Resize Map");
}

}  // namespace Tactile

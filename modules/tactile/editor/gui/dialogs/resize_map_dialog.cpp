#include "resize_map_dialog.hpp"

#include <imgui.h>

#include "editor/events/map_events.hpp"
#include "editor/gui/common/button.hpp"

namespace Tactile {

ResizeMapDialog::ResizeMapDialog() : ADialog{"Resize Map"} {}

void ResizeMapDialog::Show(const usize nCurrentRows, const usize nCurrentColumns)
{
  mRows = nCurrentRows;
  mColumns = nCurrentColumns;
  ADialog::Show();
}

void ResizeMapDialog::UpdateContents(const entt::registry&, entt::dispatcher&)
{
  auto rows = static_cast<int>(mRows);
  ImGui::DragInt("Rows", &rows, 1.0f, 1, 10'000);
  mRows = static_cast<usize>(rows);

  auto cols = static_cast<int>(mColumns);
  ImGui::DragInt("Columns", &cols, 1.0f, 1, 10'000);
  mColumns = static_cast<usize>(cols);
}

void ResizeMapDialog::OnAccept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ResizeMapEvent>(mRows, mColumns);
}

}  // namespace Tactile

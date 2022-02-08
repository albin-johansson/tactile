#include "resize_map_dialog.hpp"

#include <imgui.h>

#include "editor/events/map_events.hpp"
#include "editor/gui/common/button.hpp"

namespace tactile {

resize_map_dialog::resize_map_dialog() : dialog_base{"Resize Map"} {}

void resize_map_dialog::show(usize nCurrentRows, usize nCurrentColumns)
{
  mRows = nCurrentRows;
  mColumns = nCurrentColumns;
  make_visible();
}

void resize_map_dialog::on_update(const Model&, entt::dispatcher&)
{
  auto rows = static_cast<int>(mRows);
  ImGui::DragInt("Rows", &rows, 1.0f, 1, 10'000);
  mRows = static_cast<usize>(rows);

  auto cols = static_cast<int>(mColumns);
  ImGui::DragInt("Columns", &cols, 1.0f, 1, 10'000);
  mColumns = static_cast<usize>(cols);
}

void resize_map_dialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<resize_map_event>(mRows, mColumns);
}

}  // namespace tactile

// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "resize_map_dialog.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/event/map_events.hpp"
#include "ui/constants.hpp"
#include "ui/dialog/dialog.hpp"

namespace tactile::ui {
namespace {

struct ResizeMapDialogState final {
  uint64 row_count {};
  uint64 column_count {};
  bool open_dialog {};
};

inline constinit ResizeMapDialogState gDialogState;

}  // namespace

void open_resize_map_dialog(const TileExtent current_extent)
{
  gDialogState.row_count = current_extent.rows;
  gDialogState.column_count = current_extent.cols;
  gDialogState.open_dialog = true;
}

void update_resize_map_dialog(entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  DialogOptions options {
      .title = lang.window.resize_map.c_str(),
      .close_label = lang.misc.close.c_str(),
      .accept_label = lang.misc.ok.c_str(),
      .flags = UI_DIALOG_FLAG_INPUT_IS_VALID,
  };

  if (gDialogState.open_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    gDialogState.open_dialog = false;
  }

  DialogAction action {DialogAction::None};
  if (const ScopedDialog dialog {options, &action}; dialog.was_opened()) {
    const uint64 min_value = 1;
    const uint64 max_value = 10'000;

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.misc.rows.c_str());
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-kMinFloat);
    ImGui::DragScalar("##Rows",
                      ImGuiDataType_U64,
                      &gDialogState.row_count,
                      1.0f,
                      &min_value,
                      &max_value);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.misc.columns.c_str());
    ImGui::SameLine();
    ImGui::DragScalar("##Columns",
                      ImGuiDataType_U64,
                      &gDialogState.column_count,
                      1.0f,
                      &min_value,
                      &max_value);
  }

  if (action == DialogAction::Accept) {
    dispatcher.enqueue<ResizeMapEvent>(gDialogState.row_count, gDialogState.column_count);
  }
}

}  // namespace tactile::ui

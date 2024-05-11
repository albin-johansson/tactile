// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/dialog/new_map_dialog.hpp"

#include <imgui.h>

#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/event/map_events.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/core/ui/common/buttons.hpp"
#include "tactile/core/ui/common/popups.hpp"
#include "tactile/core/ui/common/style.hpp"
#include "tactile/core/ui/common/window.hpp"
#include "tactile/core/ui/i18n/language.hpp"
#include "tactile/core/ui/imgui_compat.hpp"

namespace tactile::ui {
inline namespace new_map_dialog {

void _push_map_orientation_combo(const Language& language,
                                 MapOrientation& orientation)
{
  const auto* orthogonal = language.get(StringID::kOrthogonal);
  const auto* hexagonal = language.get(StringID::kHexagonal);

  const auto* orientation_preview =
      (orientation == MapOrientation::kOrthogonal) ? orthogonal : hexagonal;

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(language.get(StringID::kOrientation));
  ImGui::SameLine();

  if (ImGui::BeginCombo("##MapOrientation", orientation_preview)) {
    if (ImGui::Selectable(orthogonal)) {
      orientation = MapOrientation::kOrthogonal;
    }

    if (ImGui::Selectable(hexagonal)) {
      orientation = MapOrientation::kHexagonal;
    }

    ImGui::EndCombo();
  }
}

void _push_row_count_input(const Language& language,
                           const float offset,
                           MatrixExtent::value_type& rows)
{
  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(language.get(StringID::kRows));
  ImGui::SameLine(offset);
  ImGui::SetNextItemWidth(-1.0f);
  ImGui::InputScalar("##Rows",
                     to_imgui_data_type<MatrixExtent::value_type>(),
                     &rows);
}

void _push_column_count_input(const Language& language,
                              const float offset,
                              MatrixExtent::value_type& columns)
{
  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(language.get(StringID::kColumns));
  ImGui::SameLine(offset);
  ImGui::SetNextItemWidth(-1.0f);
  ImGui::InputScalar("##Columns",
                     to_imgui_data_type<MatrixExtent::value_type>(),
                     &columns);
}

void _push_tile_width_input(const Language& language,
                            const float offset,
                            Int2::value_type& tile_width)
{
  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(language.get(StringID::kTileWidth));
  ImGui::SameLine(offset);
  ImGui::SetNextItemWidth(-1.0f);
  ImGui::InputScalar("##TileWidth",
                     to_imgui_data_type<Int2::value_type>(),
                     &tile_width);
}

void _push_tile_height_input(const Language& language,
                             const float offset,
                             Int2::value_type& tile_height)
{
  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(language.get(StringID::kTileHeight));
  ImGui::SameLine(offset);
  ImGui::SetNextItemWidth(-1.0f);
  ImGui::InputScalar("##TileHeight",
                     to_imgui_data_type<Int2::value_type>(),
                     &tile_height);
}

}  // namespace new_map_dialog

void NewMapDialog::push(const Model& model, EventDispatcher& dispatcher)
{
  const auto& language = model.get_language();
  const auto* dialog_name = language.get(StringID::kCreateMap);

  center_next_window(ImGuiCond_Always);

  if (const PopupScope new_map_dialog {kModalPopup,
                                       dialog_name,
                                       ImGuiWindowFlags_AlwaysAutoResize};
      new_map_dialog.is_open()) {
    const auto widget_offset =
        get_alignment_offset(language.get(StringID::kOrientation),
                             language.get(StringID::kRows),
                             language.get(StringID::kColumns),
                             language.get(StringID::kTileWidth),
                             language.get(StringID::kTileHeight));

    _push_map_orientation_combo(language, mSpec.orientation);

    ImGui::Spacing();

    _push_row_count_input(language, widget_offset, mSpec.extent.rows);
    _push_column_count_input(language, widget_offset, mSpec.extent.cols);

    ImGui::Spacing();

    _push_tile_width_input(language, widget_offset, mSpec.tile_size[0]);
    _push_tile_height_input(language, widget_offset, mSpec.tile_size[1]);

    ImGui::Spacing();

    if (push_button(language.get(StringID::kCancel))) {
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (push_button(language.get(StringID::kCreate),
                    nullptr,
                    is_valid(mSpec))) {
      dispatcher.push<CreateMapEvent>(mSpec);
      ImGui::CloseCurrentPopup();
    }
  }

  if (mShouldOpen) {
    ImGui::OpenPopup(dialog_name, ImGuiPopupFlags_NoReopen);
    mShouldOpen = false;
  }
}

void NewMapDialog::open()
{
  mSpec = MapSpec {
    .orientation = MapOrientation::kOrthogonal,
    .extent = MatrixExtent {10, 10},
    .tile_size = Int2 {32, 32},
  };

  mShouldOpen = true;
}

}  // namespace tactile::ui

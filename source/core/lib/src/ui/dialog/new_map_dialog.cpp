// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/dialog/new_map_dialog.hpp"

#include <imgui.h>

#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/event/events.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/core/ui/common/buttons.hpp"
#include "tactile/core/ui/common/dialogs.hpp"
#include "tactile/core/ui/common/popups.hpp"
#include "tactile/core/ui/common/style.hpp"
#include "tactile/core/ui/common/widgets.hpp"
#include "tactile/core/ui/common/window.hpp"
#include "tactile/core/ui/i18n/language.hpp"
#include "tactile/core/ui/imgui_compat.hpp"

namespace tactile::core::ui {
namespace {

void _push_map_orientation_combo(const Language& language, TileOrientation& orientation)
{
  const auto* orthogonal = language.get(AdjectiveLabel::kOrthogonal);
  const auto* hexagonal = language.get(AdjectiveLabel::kHexagonal);

  const auto* orientation_preview =
      (orientation == TileOrientation::kOrthogonal) ? orthogonal : hexagonal;

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(language.get(NounLabel::kOrientation));
  ImGui::SameLine();

  if (ImGui::BeginCombo("##TileOrientation", orientation_preview)) {
    if (ImGui::Selectable(orthogonal)) {
      orientation = TileOrientation::kOrthogonal;
    }

    if (ImGui::Selectable(hexagonal)) {
      orientation = TileOrientation::kHexagonal;
    }

    ImGui::EndCombo();
  }
}

}  // namespace

void NewMapDialog::push(const Model& model, EventDispatcher& dispatcher)
{
  const auto& language = model.get_language();
  const auto* dialog_name = language.get(ActionLabel::kCreateMap);

  center_next_window(ImGuiCond_Always);

  if (const PopupScope new_map_dialog {kModalPopup,
                                       dialog_name,
                                       ImGuiWindowFlags_AlwaysAutoResize};
      new_map_dialog.is_open()) {
    const auto widget_offset = get_alignment_offset(language.get(NounLabel::kOrientation),
                                                    language.get(NounLabel::kRows),
                                                    language.get(NounLabel::kColumns),
                                                    language.get(NounLabel::kTileWidth),
                                                    language.get(NounLabel::kTileHeight));

    _push_map_orientation_combo(language, mSpec.orientation);

    ImGui::Spacing();

    push_scalar_input_row(language.get(NounLabel::kRows), mSpec.extent.rows, widget_offset);
    push_scalar_input_row(language.get(NounLabel::kColumns), mSpec.extent.cols, widget_offset);

    ImGui::Spacing();

    push_scalar_input_row(language.get(NounLabel::kTileWidth),
                          mSpec.tile_size[0],
                          widget_offset);
    push_scalar_input_row(language.get(NounLabel::kTileHeight),
                          mSpec.tile_size[1],
                          widget_offset);

    const auto status = push_dialog_control_buttons(language.get(VerbLabel::kCancel),
                                                    language.get(VerbLabel::kCreate),
                                                    nullptr,
                                                    is_valid(mSpec));
    if (status == DialogStatus::kAccepted) {
      dispatcher.push<CreateMapEvent>(mSpec);
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
    .orientation = TileOrientation::kOrthogonal,
    .extent = Extent2D {10, 10},
    .tile_size = Int2 {32, 32},
  };

  mShouldOpen = true;
}

}  // namespace tactile::core::ui

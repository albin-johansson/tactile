// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/dialog/new_tileset_dialog.hpp"

#include <algorithm>   // max
#include <filesystem>  // relative
#include <utility>     // move

#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/event/events.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/core/platform/file_dialog.hpp"
#include "tactile/core/ui/common/buttons.hpp"
#include "tactile/core/ui/common/dialogs.hpp"
#include "tactile/core/ui/common/popups.hpp"
#include "tactile/core/ui/common/style.hpp"
#include "tactile/core/ui/common/widgets.hpp"
#include "tactile/core/ui/i18n/language.hpp"

namespace tactile::core::ui {

void NewTilesetDialog::push(const Model& model, EventDispatcher& dispatcher)
{
  const auto& language = model.get_language();
  const auto* dialog_name = language.get(StringID::kCreateTileset);

  if (const PopupScope popup {kModalPopup, dialog_name}; popup.is_open()) {
    ImGui::TextUnformatted(language.get(StringID::kSelectTilesetImage));
    ImGui::Spacing();

    if (push_button(language.get(StringID::kSelectImage))) {
      if (auto path = FileDialog::open_image()) {
        mTexturePath = std::move(*path);
        mTexturePathPreview = std::filesystem::relative(mTexturePath).string();
      }
    }

    ImGui::SameLine();

    auto input_offset = get_alignment_offset(language.get(StringID::kTileWidth),
                                             language.get(StringID::kTileHeight));
    input_offset = std::max(input_offset, ImGui::GetCursorPosX());

    {
      const DisabledScope disabled {};
      ImGui::SetNextItemWidth(-1.0f);
      ImGui::InputText("##Path",
                       mTexturePathPreview.data(),
                       mTexturePathPreview.size(),
                       ImGuiInputTextFlags_ReadOnly);
    }

    push_scalar_input_row(language.get(StringID::kTileWidth), mTileSize[0], input_offset);

    push_scalar_input_row(language.get(StringID::kTileHeight), mTileSize[1], input_offset);

    const auto can_accept = !mTexturePath.empty() &&  //
                            (mTileSize.x() > 0) &&    //
                            (mTileSize.y() > 0);

    const auto status = push_dialog_control_buttons(language.get(StringID::kCancel),
                                                    language.get(StringID::kCreate),
                                                    nullptr,
                                                    can_accept);
    if (status == DialogStatus::kAccepted) {
      dispatcher.push<AddTilesetEvent>(std::move(mTexturePath), mTileSize);
    }
  }

  if (mShouldOpen) {
    ImGui::OpenPopup(dialog_name, ImGuiPopupFlags_NoReopen);
    mShouldOpen = false;
  }
}

void NewTilesetDialog::open()
{
  mTexturePathPreview.clear();
  mTexturePath.clear();
  mTileSize = Int2 {32, 32};
  mShouldOpen = true;
}

}  // namespace tactile::core::ui

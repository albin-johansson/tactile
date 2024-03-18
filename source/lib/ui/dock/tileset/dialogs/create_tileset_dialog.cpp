// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "create_tileset_dialog.hpp"

#include <utility>  // move

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "common/util/string_buffer.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/event/tileset_events.hpp"
#include "model/settings.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/core/numeric/vec.hpp"
#include "tactile/core/platform/file_dialog.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

struct CreateTilesetDialogState final {
  Path image_path;
  StringBuffer image_path_preview_buffer {};
  Int2 tile_size {32, 32};
  bool open_dialog {};
};

inline CreateTilesetDialogState gDialogState;

void select_image_file()
{
  auto image_path = FileDialog::open_image();
  if (!image_path.has_value()) {
    return;
  }

  gDialogState.image_path = std::move(*image_path);
  const auto image_path_str = gDialogState.image_path.string();

  if (image_path_str.size() > gDialogState.image_path_preview_buffer.size()) {
    gDialogState.image_path_preview_buffer = gDialogState.image_path.filename().string();
  }
  else {
    gDialogState.image_path_preview_buffer = image_path_str;
  }
}

}  // namespace

void open_create_tileset_dialog()
{
  gDialogState.image_path_preview_buffer.clear();
  gDialogState.image_path.clear();
  gDialogState.tile_size = get_settings().get_preferred_tile_size();
  gDialogState.open_dialog = true;
}

void update_create_tileset_dialog(entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  DialogOptions options {
    .title = lang.window.create_tileset.c_str(),
    .close_label = lang.misc.cancel.c_str(),
    .accept_label = lang.misc.create.c_str(),
  };

  if (gDialogState.open_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    gDialogState.open_dialog = false;
  }

  if (!gDialogState.image_path.empty() &&  //
      gDialogState.tile_size.x() > 0 && gDialogState.tile_size.y() > 0) {
    options.flags |= UI_DIALOG_FLAG_INPUT_IS_VALID;
  }

  DialogAction action {DialogAction::None};
  if (const ScopedDialog dialog {options, &action}; dialog.was_opened()) {
    ImGui::TextUnformatted(lang.misc.create_tileset_instruction.c_str());
    ImGui::Spacing();

    if (ui_button(lang.misc.select_image.c_str())) {
      select_image_file();
    }

    ImGui::SameLine();
    ImGui::InputTextWithHint("##Source",
                             lang.misc.tileset_image_input_hint.c_str(),
                             gDialogState.image_path_preview_buffer.data(),
                             sizeof gDialogState.image_path_preview_buffer,
                             ImGuiInputTextFlags_ReadOnly);

    ImGui::DragInt(lang.misc.tile_width.c_str(),
                   &gDialogState.tile_size[0],
                   1.0f,
                   1,
                   10'000);
    ImGui::DragInt(lang.misc.tile_height.c_str(),
                   &gDialogState.tile_size[1],
                   1.0f,
                   1,
                   10'000);
  }

  if (action == DialogAction::Accept) {
    dispatcher.enqueue<LoadTilesetEvent>(gDialogState.image_path, gDialogState.tile_size);
  }
}

}  // namespace tactile::ui

#include "create_tileset_dialog.hpp"

#include <imgui.h>

#include "core/utils/buffers.hpp"
#include "editor/events/tileset_events.hpp"
#include "io/file_dialog.hpp"
#include "io/persistence/preferences.hpp"

namespace tactile {

CreateTilesetDialog::CreateTilesetDialog() : ADialog{"Create tileset"}
{
  SetAcceptButtonLabel("Create");
}

void CreateTilesetDialog::Open()
{
  zero_buffer(mPathPreviewBuffer);
  mFullImagePath.clear();

  const auto& prefs = get_preferences();
  mTileWidth = prefs.preferred_tile_width();
  mTileHeight = prefs.preferred_tile_height();

  Show();
}

void CreateTilesetDialog::UpdateContents(const Model&, entt::dispatcher&)
{
  ImGui::TextUnformatted("Select an image which contains the tiles aligned in a grid.");
  ImGui::Spacing();

  if (ImGui::Button("Select image...")) {
    ShowImageFileDialog();
  }

  ImGui::SameLine();
  ImGui::InputTextWithHint("##Source",
                           "Source image path",
                           mPathPreviewBuffer.data(),
                           mPathPreviewBuffer.size(),
                           ImGuiInputTextFlags_ReadOnly);

  ImGui::DragInt("Tile width", &mTileWidth, 1.0f, 1, 10'000);
  ImGui::DragInt("Tile height", &mTileHeight, 1.0f, 1, 10'000);
}

void CreateTilesetDialog::OnAccept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<AddTilesetEvent>(mFullImagePath, mTileWidth, mTileHeight);
}

auto CreateTilesetDialog::IsCurrentInputValid(const Model&) const -> bool
{
  return mPathPreviewBuffer.front() != '\0' && mTileWidth > 0 && mTileHeight > 0;
}

void CreateTilesetDialog::ShowImageFileDialog()
{
  auto dialog = file_dialog::open_image();
  if (!dialog.is_okay()) {
    return;
  }

  mFullImagePath = dialog.path();
  const auto pathStr = mFullImagePath.string();

  if (pathStr.size() > mPathPreviewBuffer.size()) {
    const auto name = mFullImagePath.filename();
    copy_string_into_buffer(mPathPreviewBuffer, name.string());
  }
  else {
    copy_string_into_buffer(mPathPreviewBuffer, pathStr);
  }
}

}  // namespace tactile

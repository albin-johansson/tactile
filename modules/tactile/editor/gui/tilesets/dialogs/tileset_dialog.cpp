#include "tileset_dialog.hpp"

#include <imgui.h>

#include "core/utils/buffer_utils.hpp"
#include "editor/events/tileset_events.hpp"
#include "io/file_dialog.hpp"
#include "io/preferences.hpp"

namespace Tactile {

CreateTilesetDialog::CreateTilesetDialog() : ADialog{"Create tileset"}
{
  SetAcceptButtonLabel("Create");
}

void CreateTilesetDialog::Open()
{
  ZeroBuffer(mPathPreviewBuffer);
  mFullImagePath.clear();
  mTileWidth = Prefs::GetPreferredTileWidth();
  mTileHeight = Prefs::GetPreferredTileHeight();
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
  auto dialog = FileDialog::OpenImage();
  if (!dialog.IsOkay()) {
    return;
  }

  mFullImagePath = dialog.GetPath();
  const auto pathStr = mFullImagePath.string();

  if (pathStr.size() > mPathPreviewBuffer.size()) {
    const auto name = mFullImagePath.filename();
    CopyStringIntoBuffer(mPathPreviewBuffer, name.string());
  }
  else {
    CopyStringIntoBuffer(mPathPreviewBuffer, pathStr);
  }
}

}  // namespace Tactile

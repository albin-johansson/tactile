#include "tileset_dialog.hpp"

#include <array>       // array
#include <filesystem>  // path

#include <imgui.h>
#include <portable-file-dialogs.h>
#include <tactile_def.hpp>

#include "core/utils/buffer_utils.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/gui/alignment.hpp"
#include "editor/gui/common/button.hpp"
#include "editor/gui/scoped.hpp"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

constexpr auto gFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking |
                        ImGuiWindowFlags_NoCollapse;

inline std::filesystem::path gFullImagePath;
constinit std::array<char, 100> gPathPreviewBuffer{};
constinit int gTileWidth = 32;
constinit int gTileHeight = 32;

void ShowImageFileDialog()
{
  auto files =
      pfd::open_file{"Select image", "", {"Image Files", "*.png *.jpg"}}.result();

  if (files.empty()) {
    return;
  }

  gFullImagePath = files.front();
  const auto pathStr = gFullImagePath.string();

  if (pathStr.size() > gPathPreviewBuffer.size()) {
    const auto name = gFullImagePath.filename();
    CopyStringIntoBuffer(gPathPreviewBuffer, name.string());
  }
  else {
    CopyStringIntoBuffer(gPathPreviewBuffer, pathStr);
  }
}

[[nodiscard]] auto IsInputValid() -> bool
{
  return gPathPreviewBuffer.front() != '\0' && gTileWidth > 0 && gTileHeight > 0;
}

void ResetInputs()
{
  ZeroBuffer(gPathPreviewBuffer);
  gFullImagePath.clear();
  gTileWidth = Prefs::GetPreferredTileWidth();
  gTileHeight = Prefs::GetPreferredTileHeight();
}

}  // namespace

void UpdateTilesetDialog(entt::dispatcher& dispatcher)
{
  CenterNextWindowOnAppearance();
  if (Scoped::Modal modal{"Create tileset", gFlags}; modal.IsOpen()) {
    ImGui::TextUnformatted("Select an image which contains the tiles aligned in a grid.");
    ImGui::Spacing();

    if (ImGui::Button("Select image...")) {
      ShowImageFileDialog();
    }

    ImGui::SameLine();
    ImGui::InputTextWithHint("##Source",
                             "Source image path",
                             gPathPreviewBuffer.data(),
                             gPathPreviewBuffer.size(),
                             ImGuiInputTextFlags_ReadOnly);
    ImGui::DragInt("Tile width", &gTileWidth, 1.0f, 1, 10'000);
    ImGui::DragInt("Tile height", &gTileHeight, 1.0f, 1, 10'000);

    ImGui::Spacing();
    ImGui::Separator();

    if (Button("OK", nullptr, IsInputValid())) {
      dispatcher.enqueue<AddTilesetEvent>(gFullImagePath, gTileWidth, gTileHeight);

      ResetInputs();
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Close")) {
      ResetInputs();
      ImGui::CloseCurrentPopup();
    }
  }
}

void OpenTilesetDialog()
{
  ImGui::OpenPopup("Create tileset");
}

}  // namespace Tactile

#include "tileset_dialog.hpp"

#include <imgui.h>

#include <array>       // array
#include <filesystem>  // path

#include "aliases/ints.hpp"
#include "events/tilesets/add_tileset_event.hpp"
#include "gui/widgets/common/button.hpp"
#include "gui/widgets/common/file_dialog.hpp"
#include "io/preferences.hpp"
#include "utils/buffer_utils.hpp"

namespace Tactile {
namespace {

inline std::filesystem::path full_image_path;
constinit std::array<char, 100> path_preview_buffer{};
constinit int tile_width = 32;
constinit int tile_height = 32;

constinit bool show_image_file_dialog = false;

void ShowImageFileDialog()
{
  const auto filter = ".png,.jpg";
  const auto res =
      FileDialogImport("TilesetImageFileDialog", "Select image", filter);

  if (res == FileDialogResult::Success)
  {
    full_image_path = GetFileDialogSelectedPath();
    const auto pathStr = full_image_path.string();

    if (pathStr.size() > path_preview_buffer.size())
    {
      const auto name = full_image_path.filename();
      CopyStringIntoBuffer(path_preview_buffer, name.string());
    }
    else
    {
      CopyStringIntoBuffer(path_preview_buffer, pathStr);
    }

    show_image_file_dialog = false;
  }
  else if (res == FileDialogResult::Close)
  {
    show_image_file_dialog = false;
  }
}

[[nodiscard]] auto IsInputValid() -> bool
{
  return path_preview_buffer.front() != '\0' && tile_width > 0 && tile_height > 0;
}

void ResetInputs()
{
  ZeroBuffer(path_preview_buffer);
  full_image_path.clear();
  tile_width = Prefs::GetPreferredTileWidth();
  tile_height = Prefs::GetPreferredTileHeight();
}

}  // namespace

void UpdateTilesetDialog(bool* open, entt::dispatcher& dispatcher)
{
  constexpr auto flags = ImGuiWindowFlags_AlwaysAutoResize |
                         ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse;
  if (ImGui::Begin("Add tileset", open, flags))
  {
    ImGui::TextUnformatted(
        "Select an image which contains the tiles aligned in a grid.");
    ImGui::Spacing();

    ImGui::Button("Select image...");
    if (ImGui::IsItemActivated())
    {
      show_image_file_dialog = true;
    }

    ImGui::SameLine();
    ImGui::InputText("Source",
                     path_preview_buffer.data(),
                     path_preview_buffer.size(),
                     ImGuiInputTextFlags_ReadOnly);
    ImGui::InputInt("Tile width", &tile_width);
    ImGui::InputInt("Tile height", &tile_height);

    ImGui::Spacing();
    ImGui::Separator();

    Button("OK", nullptr, IsInputValid());
    if (ImGui::IsItemActivated())
    {
      dispatcher.enqueue<AddTilesetEvent>(full_image_path, tile_width, tile_height);

      ResetInputs();
      *open = false;
    }

    ImGui::SameLine();
    ImGui::Button("Close");
    if (ImGui::IsItemActivated())
    {
      ResetInputs();
      *open = false;
    }
  }

  ImGui::End();

  if (show_image_file_dialog)
  {
    ShowImageFileDialog();
  }
}

}  // namespace Tactile

#include "show_tileset_dialog.hpp"

#include <array>        // array
#include <cassert>      // assert
#include <cstring>      // memset
#include <filesystem>   // path
#include <string_view>  // string_view

#include "aliases/ints.hpp"
#include "core/events/add_tileset_event.hpp"
#include "imgui.h"
#include "widgets/button_ex.hpp"
#include "widgets/file_dialog.hpp"

namespace tactile {
namespace {

inline std::array<char, 100> path_preview_buffer;
inline std::filesystem::path full_image_path;
inline int tile_width = 32;
inline int tile_height = 32;

inline bool show_image_file_dialog = false;

void CopyStringIntoBuffer(const std::string_view str)
{
  assert(str.size() <= path_preview_buffer.size());
  std::memset(path_preview_buffer.data(), 0, path_preview_buffer.size());

  usize index = 0;
  for (const auto ch : str)
  {
    path_preview_buffer.at(index) = ch;
    ++index;
  }
}

void ShowImageFileDialog()
{
  const auto filter = ".png,.jpg";
  const auto res = FileDialog("TilesetImageFileDialog", "Select image", filter);

  if (res == FileDialogResult::Success)
  {
    full_image_path = GetFileDialogSelectedPath();
    const auto pathStr = full_image_path.string();

    if (pathStr.size() > path_preview_buffer.size())
    {
      const auto name = full_image_path.filename();
      CopyStringIntoBuffer(name.string());
    }
    else
    {
      CopyStringIntoBuffer(pathStr);
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
  return path_preview_buffer.front() != '\0' && tile_width > 0 &&
         tile_height > 0;
}

void ResetInputs()
{
  std::memset(path_preview_buffer.data(), 0, path_preview_buffer.size());
  full_image_path.clear();
  tile_width = 32;
  tile_height = 32;
}

}  // namespace

void ShowTilesetDialog(bool* open, entt::dispatcher& dispatcher)
{
  constexpr auto flags =
      ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking;
  if (ImGui::Begin("Add tileset", open, flags))
  {
    ImGui::Text("Select an image which contains the tiles aligned in a grid.");
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

    ButtonEx("OK", nullptr, IsInputValid());
    if (ImGui::IsItemActivated())
    {
      dispatcher.enqueue<AddTilesetEvent>(full_image_path,
                                          tile_width,
                                          tile_height);

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

}  // namespace tactile

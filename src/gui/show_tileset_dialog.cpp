#include "show_tileset_dialog.hpp"

#include <IconsFontAwesome5.h>

#include <array>  // array

#include "imgui.h"

namespace tactile {

void ShowTilesetDialog(bool* open)
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2{500, 130});

  if (ImGui::Begin("Add tileset", open, ImGuiWindowFlags_Modal))
  {
    ImGui::Text("Select an image which contains the tiles aligned in a grid.");

    ImGui::Spacing();
    if (ImGui::Button("Select image..."))
    {}

    ImGui::SameLine();

    static std::array<char, 100> source;
    ImGui::InputText("Source",
                     source.data(),
                     source.size(),
                     ImGuiInputTextFlags_ReadOnly);

    static int tileWidth = 32;
    static int tileHeight = 32;
    ImGui::InputInt("Tile width", &tileWidth);
    ImGui::InputInt("Tile height", &tileHeight);
  }

  ImGui::PopStyleVar();
  ImGui::End();
}

}  // namespace tactile

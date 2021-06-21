#include "show_layers.hpp"

#include <limits>  // numeric_limits

#include "imgui.h"

namespace tactile {
namespace {

inline bool layer_tree_open = true;
inline int selected_layer = 0;

}  // namespace

void ShowLayers()
{
  if (ImGui::Begin("Layers"))
  {
    ImGui::Button("New");

    ImGui::SameLine();
    ImGui::Button("Remove");

    const ImVec2 size{
        std::numeric_limits<float>::min(),
        ImGui::GetWindowHeight() - (4 * ImGui::GetTextLineHeightWithSpacing())};

    if (ImGui::BeginListBox("##LayerListBox", size))
    {
      if (ImGui::Selectable("Layer 1"))
      {
        //        selected_layer = index;
      }

      if (ImGui::Selectable("Layer 2"))
      {}

      ImGui::EndListBox();
    }
  }

  ImGui::End();
}

}  // namespace tactile

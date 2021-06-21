#include "show_layers.hpp"

#include "imgui.h"

namespace tactile {
namespace {

inline int selected_layer = 0;

}  // namespace

void ShowLayers()
{
  if (ImGui::Begin("Layers"))
  {
    ImGui::SetNextItemOpen(true);
    if (ImGui::TreeNode("##LayerTreeNode"))
    {
      for (int index = 0; index < 5; ++index)
      {
        if (ImGui::Selectable("Layer", selected_layer == index))
        {
          selected_layer = index;
        }
      }

      ImGui::TreePop();
    }
  }

  ImGui::End();
}

}  // namespace tactile

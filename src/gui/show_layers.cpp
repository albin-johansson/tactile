#include "show_layers.hpp"

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
    ImGui::SetNextItemOpen(layer_tree_open);
    layer_tree_open = ImGui::TreeNode("##LayerTreeNode");

    if (layer_tree_open)
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

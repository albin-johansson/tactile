#include "layer_widget.hpp"

#include <IconsFontAwesome5.h>

#include <limits>  // numeric_limits

#include "gui/widgets/button_ex.hpp"
#include "imgui.h"

namespace tactile {
namespace {

inline bool layer_tree_open = true;
inline int selected_layer = 0;

}  // namespace

void UpdateLayerWidget()
{
  if (ImGui::Begin("Layers"))
  {
    if (ButtonEx(ICON_FA_PLUS_CIRCLE, "Add new layer."))
    {}

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_MINUS_CIRCLE, "Remove layer."))
    {}

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_COPY, "Duplicate layer."))
    {}

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_ARROW_UP, "Move layer up."))
    {}

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_ARROW_DOWN, "Move layer down."))
    {}

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

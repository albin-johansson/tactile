#include "component_widget.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "editor/gui/alignment.hpp"
#include "editor/gui/common/input_widgets.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"

namespace Tactile {
namespace {

[[nodiscard]] auto IntAttribute(const CStr name, int value) -> int
{
  const Scoped::ID scope{name};

  ImGui::TableNextRow();
  ImGui::TableNextColumn();
  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(name);

  ImGui::TableNextColumn();
  return InputWidget("##IntAttribute", value).value_or(value);
}

}  // namespace

void ComponentWidget(const CStr name)
{
  const Scoped::ID scope{name};

  ImGui::Separator();

  constexpr auto header_flags =
      ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

  if (ImGui::TreeNodeEx(name, header_flags)) {
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32_BLACK_TRANS);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32_BLACK_TRANS);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32_BLACK_TRANS);
    AlignNextItemToTheRight(TAC_ICON_THREE_DOTS);
    if (ImGui::SmallButton(TAC_ICON_THREE_DOTS)) {
      ImGui::OpenPopup("##ComponentPopup");
    }
    ImGui::PopStyleColor(3);

    if (ImGui::BeginPopupContextItem("##ComponentPopup")) {
      ImGui::MenuItem(TAC_ICON_RESET " Reset Values");

      ImGui::Separator();

      ImGui::MenuItem(TAC_ICON_COPY " Copy Component");

      ImGui::Separator();

      ImGui::MenuItem(TAC_ICON_REMOVE " Remove Component");

      ImGui::EndPopup();
    }

    constexpr auto table_flags =
        ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_PadOuterX;

    if (Scoped::Table table{"##ComponentTable", 2, table_flags}; table.IsOpen()) {
      static int texture = 0;
      texture = IntAttribute("Texture", texture);

      static int layer = 0;
      layer = IntAttribute("Layer", layer);

      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::AlignTextToFramePadding();
      ImGui::TextUnformatted("Center");

      static int center[2] = {};
      ImGui::TableNextColumn();
      ImGui::InputInt2("##Center", center);
    }

    ImGui::TreePop();
  }
}

}  // namespace Tactile

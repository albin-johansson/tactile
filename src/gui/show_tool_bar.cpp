#include "show_tool_bar.hpp"

#include <IconsFontAwesome5.h>

#include "core/events/add_map_event.hpp"
#include "core/events/redo_event.hpp"
#include "core/events/undo_event.hpp"
#include "core/model.hpp"
#include "imgui.h"
#include "widgets/button_ex.hpp"

namespace tactile {

void ShowToolBar(const Model& model, entt::dispatcher& dispatcher)
{
  const auto* document = model.GetActiveDocument();
  if (ImGui::Begin("Toolbar"))
  {
    if (ButtonEx(ICON_FA_PLUS, "Create new tilemap."))
    {
      dispatcher.enqueue<AddMapEvent>();
    }

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_FOLDER_OPEN, "Open tilemap."))
    {
      cen::log::info("ShowToolBar > Open!");
    }

    ImGui::SameLine();
    ImGui::Spacing();

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_UNDO, "Undo", document && document->CanUndo()))
    {
      dispatcher.enqueue<UndoEvent>();
    }

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_REDO, "Redo", document && document->CanRedo()))
    {
      dispatcher.enqueue<RedoEvent>();
    }

    ImGui::SameLine();
    ImGui::Spacing();
  }

  ImGui::End();
}

}  // namespace tactile

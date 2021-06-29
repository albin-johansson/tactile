#include "tool_bar.hpp"

#include <IconsFontAwesome5.h>

#include "core/events/add_map_event.hpp"
#include "core/events/redo_event.hpp"
#include "core/events/undo_event.hpp"
#include "core/model.hpp"
#include "gui/widgets/button_ex.hpp"
#include "gui/widgets/menus/menu_bar_widget.hpp"
#include "imgui.h"

namespace Tactile {
namespace {

inline constexpr auto flags =
    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
    ImGuiWindowFlags_NoScrollbar;

}  // namespace

void UpdateToolbarWidget(const Model& model, entt::dispatcher& dispatcher)
{
  const auto* document = model.GetActiveDocument();
  if (ImGui::Begin("Toolbar", nullptr, flags))
  {
    if (ButtonEx(ICON_FA_FILE, "Create new tilemap."))
    {
      dispatcher.enqueue<AddMapEvent>();
    }

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_FOLDER_OPEN, "Open tilemap."))
    {
      cen::log::info("UpdateToolbarWidget > Open!");
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

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_IMAGE, "Create tileset.", document))
    {
      EnableTilesetDialog();
    }

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_CROP_ALT, "Resize map.", document))
    {}

    ImGui::SameLine();
    ImGui::Spacing();

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_STAMP, "Stamp tool.", document))
    {}

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_FILL, "Bucket tool.", document))
    {}

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_ERASER, "Eraser tool.", document))
    {}
  }

  ImGui::End();
}

}  // namespace Tactile

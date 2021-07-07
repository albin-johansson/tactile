#include "tool_bar.hpp"

#include <imgui.h>

#include "core/events/maps/add_map_event.hpp"
#include "core/events/redo_event.hpp"
#include "core/events/tools/select_tool_event.hpp"
#include "core/events/undo_event.hpp"
#include "core/model.hpp"
#include "gui/icons.hpp"
#include "gui/widgets/common/button_ex.hpp"
#include "gui/widgets/menus/edit_menu.hpp"
#include "gui/widgets/toolbar/tool_button.hpp"

namespace Tactile {
namespace {

inline constexpr auto flags = ImGuiWindowFlags_AlwaysAutoResize |
                              ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar |
                              ImGuiWindowFlags_NoCollapse |
                              ImGuiWindowFlags_NoScrollbar;

}  // namespace

void UpdateToolbarWidget(const Model& model, entt::dispatcher& dispatcher)
{
  const auto* document = model.GetActiveDocument();
  if (ImGui::Begin("Toolbar", nullptr, flags))
  {
    if (ButtonEx(TAC_ICON_FILE, "Create new tilemap."))
    {
      dispatcher.enqueue<AddMapEvent>();
    }

    ImGui::SameLine();
    if (ButtonEx(TAC_ICON_OPEN, "Open tilemap."))
    {
      // TODO
    }

    ImGui::SameLine();
    ImGui::Spacing();

    ImGui::SameLine();
    if (ButtonEx(TAC_ICON_UNDO, "Undo", document && document->CanUndo()))
    {
      dispatcher.enqueue<UndoEvent>();
    }

    ImGui::SameLine();
    if (ButtonEx(TAC_ICON_REDO, "Redo", document && document->CanRedo()))
    {
      dispatcher.enqueue<RedoEvent>();
    }

    ImGui::SameLine();
    ImGui::Spacing();

    ImGui::SameLine();
    if (ButtonEx(TAC_ICON_TILESET, "Create tileset.", document))
    {
      EnableTilesetDialog();
    }

    ImGui::SameLine();
    if (ButtonEx(TAC_ICON_RESIZE, "Resize map.", document))
    {}

    ImGui::SameLine();
    ImGui::Spacing();

    ImGui::SameLine();
    if (ToolButton(TAC_ICON_STAMP,
                   "Stamp tool.",
                   model.IsStampActive(),
                   document != nullptr))
    {
      dispatcher.enqueue<SelectToolEvent>(MouseToolType::Stamp);
    }

    ImGui::SameLine();
    if (ToolButton(TAC_ICON_BUCKET,
                   "Bucket tool.",
                   model.IsBucketActive(),
                   document != nullptr))
    {
      dispatcher.enqueue<SelectToolEvent>(MouseToolType::Bucket);
    }

    ImGui::SameLine();
    if (ToolButton(TAC_ICON_ERASER,
                   "Eraser tool.",
                   model.IsEraserActive(),
                   document != nullptr))
    {
      dispatcher.enqueue<SelectToolEvent>(MouseToolType::Eraser);
    }
  }

  ImGui::End();
}

}  // namespace Tactile

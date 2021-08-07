#include "tool_bar.hpp"

#include <imgui.h>

#include "core/model.hpp"
#include "events/redo_event.hpp"
#include "events/tools/select_tool_event.hpp"
#include "events/undo_event.hpp"
#include "gui/icons.hpp"
#include "gui/widgets/common/button.hpp"
#include "gui/widgets/menus/edit_menu.hpp"
#include "gui/widgets/menus/file_menu.hpp"
#include "gui/widgets/toolbar/tool_button.hpp"

namespace Tactile {
namespace {

constexpr auto flags = ImGuiWindowFlags_AlwaysAutoResize |
                              ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar |
                              ImGuiWindowFlags_NoCollapse |
                              ImGuiWindowFlags_NoScrollbar;

}  // namespace

void UpdateToolbarWidget(const Model& model, entt::dispatcher& dispatcher)
{
  const auto* document = model.GetActiveDocument();
  if (ImGui::Begin("Toolbar", nullptr, flags))
  {
    if (Button(TAC_ICON_FILE, "Create new tilemap."))
    {
      EnableAddMapDialog();
    }

    ImGui::SameLine();
    if (Button(TAC_ICON_OPEN, "Open tilemap."))
    {
      EnableOpenMapDialog();
    }

    ImGui::SameLine();
    ImGui::Spacing();

    ImGui::SameLine();
    if (Button(TAC_ICON_UNDO, "Undo", document && document->CanUndo()))
    {
      dispatcher.enqueue<UndoEvent>();
    }

    ImGui::SameLine();
    if (Button(TAC_ICON_REDO, "Redo", document && document->CanRedo()))
    {
      dispatcher.enqueue<RedoEvent>();
    }

    ImGui::SameLine();
    ImGui::Spacing();

    ImGui::SameLine();
    if (Button(TAC_ICON_TILESET, "Create tileset.", document))
    {
      EnableTilesetDialog();
    }

    ImGui::SameLine();
    if (Button(TAC_ICON_RESIZE, "Resize map.", document))
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

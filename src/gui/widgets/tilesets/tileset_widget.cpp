#include "tileset_widget.hpp"

#include <IconsFontAwesome5.h>

#include "core/events/remove_tileset_event.hpp"
#include "core/model.hpp"
#include "gui/widgets/button_ex.hpp"
#include "gui/widgets/menus/menu_bar_widget.hpp"
#include "gui/widgets/tilesets/tileset_content_widget.hpp"
#include "imgui.h"

namespace Tactile {
namespace {

inline bool is_visible = true;

}  // namespace

void UpdateTilesetWidget(const Model& model, entt::dispatcher& dispatcher)
{
  if (!is_visible || !model.GetActiveMapId())
  {
    return;
  }

  const auto* document = model.GetActiveDocument();
  const auto& tilesets = document->GetTilesets();

  if (ImGui::Begin("Tilesets",
                   &is_visible,
                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
  {
    if (ButtonEx(ICON_FA_PLUS_CIRCLE, "Create tileset."))
    {
      EnableTilesetDialog();
    }

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_MINUS_CIRCLE,
                 "Remove tileset.",
                 tilesets.HasActiveTileset()))
    {
      dispatcher.enqueue<RemoveTilesetEvent>(*tilesets.GetActiveTilesetId());
    }

    if (tilesets.GetSize() != 0)
    {
      TilesetContentWidget(*document, dispatcher);
    }
    else
    {
      ImGui::Text("No tilesets available!");
    }
  }

  ImGui::End();
}

void SetTilesetWidgetVisible(const bool visible) noexcept
{
  is_visible = visible;
}

auto IsTilesetWidgetVisible() noexcept -> bool
{
  return is_visible;
}

}  // namespace Tactile

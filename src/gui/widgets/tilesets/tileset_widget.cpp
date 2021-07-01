#include "tileset_widget.hpp"

#include <IconsFontAwesome5.h>

#include "core/events/remove_tileset_event.hpp"
#include "core/model.hpp"
#include "gui/widgets/alignment.hpp"
#include "gui/widgets/centered_button.hpp"
#include "gui/widgets/centered_text.hpp"
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
    if (tilesets.GetSize() != 0)
    {
      TilesetContentWidget(*document, dispatcher);
    }
    else
    {
      PrepareVerticalAlignmentCenter(2);

      CenteredText("No available tilesets!");
      if (CenteredButton(ICON_FA_IMAGE " Create tileset..."))
      {
        EnableTilesetDialog();
      }
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

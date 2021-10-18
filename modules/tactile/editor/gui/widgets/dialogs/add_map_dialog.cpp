#include "add_map_dialog.hpp"

#include <imgui.h>

#include "tactile/editor/events/map_events.hpp"
#include "tactile/editor/gui/widgets/alignment.hpp"
#include "tactile/editor/gui/widgets/common/button.hpp"
#include "tactile/editor/gui/widgets/common/modal.hpp"
#include "tactile/io/preferences.hpp"

namespace Tactile {
namespace {

constexpr auto flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

constinit int tile_width = 32;
constinit int tile_height = 32;

void ResetState()
{
  tile_width = Prefs::GetPreferredTileWidth();
  tile_height = Prefs::GetPreferredTileHeight();
}

[[nodiscard]] auto IsInputValid() noexcept -> bool
{
  return tile_width > 0 && tile_height > 0;
}

}  // namespace

void UpdateAddMapDialog(entt::dispatcher& dispatcher)
{
  CenterNextWindowOnAppearance();
  if (auto modal = Modal{"Add map", flags}) {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Tile width:  ");
    ImGui::SameLine();
    ImGui::DragInt("##TileWidthInput", &tile_width, 1.0f, 1, 10'000);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Tile height: ");
    ImGui::SameLine();
    ImGui::DragInt("##TileHeightInput", &tile_height, 1.0f, 1, 10'000);

    ImGui::Spacing();
    if (Button("OK", nullptr, IsInputValid())) {
      dispatcher.enqueue<AddMapEvent>(tile_width, tile_height);

      ResetState();
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (Button("Cancel")) {
      ResetState();
      ImGui::CloseCurrentPopup();
    }
  }
}

void OpenAddMapDialog()
{
  ResetState();
  ImGui::OpenPopup("Add map");
}

}  // namespace Tactile

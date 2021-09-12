#include "add_map_dialog.hpp"

#include <imgui.h>

#include "events/map_events.hpp"
#include "gui/widgets/alignment.hpp"
#include "gui/widgets/common/button.hpp"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

constexpr auto flags =
    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

constinit int tile_width = 32;
constinit int tile_height = 32;

void ResetState()
{
  tile_width = Prefs::GetPreferredTileWidth();
  tile_height = Prefs::GetPreferredTileHeight();
}

[[nodiscard]] auto is_input_valid() noexcept -> bool
{
  return tile_width > 0 && tile_height > 0;
}

}  // namespace

void UpdateAddMapDialog(entt::dispatcher& dispatcher)
{
  CenterNextWindowOnAppearance();
  if (ImGui::BeginPopupModal("Add map", nullptr, flags))
  {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Tile width:  ");
    ImGui::SameLine();
    ImGui::InputInt("##TileWidthInput", &tile_width);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Tile height: ");
    ImGui::SameLine();
    ImGui::InputInt("##TileHeightInput", &tile_height);

    ImGui::Spacing();
    if (Button("OK", nullptr, is_input_valid()))
    {
      dispatcher.enqueue<AddMapEvent>(tile_width, tile_height);

      ResetState();
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (Button("Cancel"))
    {
      ResetState();
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
}

void OpenAddMapDialog()
{
  ResetState();
  ImGui::OpenPopup("Add map");
}

}  // namespace Tactile

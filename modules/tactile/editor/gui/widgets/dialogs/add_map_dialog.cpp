#include "add_map_dialog.hpp"

#include <imgui.h>

#include "editor/events/map_events.hpp"
#include "editor/gui/widgets/alignment.hpp"
#include "editor/gui/widgets/common/button.hpp"
#include "editor/gui/widgets/common/modal.hpp"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

constexpr auto gFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

constinit int gTileWidth = 32;
constinit int gTileHeight = 32;

void ResetState()
{
  gTileWidth = Prefs::GetPreferredTileWidth();
  gTileHeight = Prefs::GetPreferredTileHeight();
}

[[nodiscard]] auto IsInputValid() noexcept -> bool
{
  return gTileWidth > 0 && gTileHeight > 0;
}

}  // namespace

void UpdateAddMapDialog(entt::dispatcher& dispatcher)
{
  CenterNextWindowOnAppearance();
  if (auto modal = Modal{"Add map", gFlags}) {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Tile width:  ");
    ImGui::SameLine();
    ImGui::DragInt("##TileWidthInput", &gTileWidth, 1.0f, 1, 10'000);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Tile height: ");
    ImGui::SameLine();
    ImGui::DragInt("##TileHeightInput", &gTileHeight, 1.0f, 1, 10'000);

    ImGui::Spacing();
    if (Button("OK", nullptr, IsInputValid())) {
      dispatcher.enqueue<AddMapEvent>(gTileWidth, gTileHeight);

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

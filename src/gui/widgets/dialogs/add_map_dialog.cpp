#include "add_map_dialog.hpp"

#include <imgui.h>

#include "core/events/maps/add_map_event.hpp"
#include "gui/widgets/common/button_ex.hpp"

namespace Tactile {
namespace {

constexpr auto flags =
    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

constinit int tile_width = 32;
constinit int tile_height = 32;

void ResetState()
{
  tile_width = 32;   // TODO use preference
  tile_height = 32;  // TODO use preference
}

[[nodiscard]] auto is_input_valid() noexcept -> bool
{
  return tile_width > 0 && tile_height > 0;
}

}  // namespace

void UpdateAddMapDialog(entt::dispatcher& dispatcher)
{
  if (ImGui::BeginPopupModal("Add map", nullptr, flags))
  {
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Tile width:  ");
    ImGui::SameLine();
    ImGui::InputInt("##TileWidthLabel", &tile_width);

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Tile height: ");
    ImGui::SameLine();
    ImGui::InputInt("##TileHeightLabel", &tile_height);

    ImGui::Spacing();
    if (ButtonEx("OK", nullptr, is_input_valid()))
    {
      dispatcher.enqueue<AddMapEvent>(tile_width, tile_height);

      ResetState();
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (ButtonEx("Cancel"))
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

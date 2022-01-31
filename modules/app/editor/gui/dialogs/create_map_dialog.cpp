#include "create_map_dialog.hpp"

#include <imgui.h>

#include "assert.hpp"
#include "editor/events/map_events.hpp"
#include "editor/gui/alignment.hpp"
#include "io/preferences.hpp"
#include "tactile_def.hpp"

namespace tactile {

CreateMapDialog::CreateMapDialog() : ADialog{"Create New Map"}
{
  SetAcceptButtonLabel("Create");
}

void CreateMapDialog::Open()
{
  ADialog::Show();

  const auto& prefs = get_preferences();
  mTileWidth = prefs.preferred_tile_width();
  mTileHeight = prefs.preferred_tile_height();

  mRows = 5;
  mColumns = 5;
}

void CreateMapDialog::UpdateContents(const Model&, entt::dispatcher&)
{
  {
    c_str rowsLabel = "Rows:";
    c_str columnsLabel = "Columns:";
    const auto offset = GetMinimumOffsetToAlign(rowsLabel, columnsLabel);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(rowsLabel);
    ImGui::SameLine(offset);
    ImGui::InputInt("##Rows", &mRows);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(columnsLabel);
    ImGui::SameLine(offset);
    ImGui::InputInt("##Columns", &mColumns);
  }

  ImGui::Separator();

  {
    c_str tileWidthLabel = "Tile width:";
    c_str tileHeightLabel = "Tile height:";
    const auto offset = GetMinimumOffsetToAlign(tileWidthLabel, tileHeightLabel);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(tileWidthLabel);
    ImGui::SameLine(offset);
    ImGui::InputInt("##TileWidth", &mTileWidth);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(tileHeightLabel);
    ImGui::SameLine(offset);
    ImGui::InputInt("##TileHeight", &mTileHeight);
  }
}

void CreateMapDialog::OnAccept(entt::dispatcher& dispatcher)
{
  TACTILE_ASSERT(mTileWidth > 0);
  TACTILE_ASSERT(mTileHeight > 0);
  TACTILE_ASSERT(mRows > 0);
  TACTILE_ASSERT(mColumns > 0);
  dispatcher.enqueue<CreateMapEvent>(mTileWidth,
                                     mTileHeight,
                                     static_cast<usize>(mRows),
                                     static_cast<usize>(mColumns));
}

auto CreateMapDialog::IsCurrentInputValid(const Model&) const -> bool
{
  return (mTileWidth > 0) && (mTileHeight > 0) && (mRows > 0) && (mColumns > 0);
}

}  // namespace tactile

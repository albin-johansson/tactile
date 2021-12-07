#include "create_map_dialog.hpp"

#include <cassert>  // assert

#include <tactile_def.hpp>

#include <imgui.h>

#include "editor/events/map_events.hpp"
#include "editor/gui/alignment.hpp"
#include "io/preferences.hpp"

namespace Tactile {

CreateMapDialog::CreateMapDialog() : ADialog{"Create New Map"}
{
  SetAcceptButtonLabel("Create");
}

void CreateMapDialog::Open()
{
  ADialog::Show();
  mTileWidth = Prefs::GetPreferredTileWidth();
  mTileHeight = Prefs::GetPreferredTileHeight();
  mRows = 5;
  mColumns = 5;
}

void CreateMapDialog::UpdateContents(const entt::registry&, entt::dispatcher&)
{
  {
    CStr rowsLabel = "Rows:";
    CStr columnsLabel = "Columns:";
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
    CStr tileWidthLabel = "Tile width:";
    CStr tileHeightLabel = "Tile height:";
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
  assert(mTileWidth > 0);
  assert(mTileHeight > 0);
  assert(mRows > 0);
  assert(mColumns > 0);
  dispatcher.enqueue<CreateMapEvent>(mTileWidth,
                                     mTileHeight,
                                     static_cast<usize>(mRows),
                                     static_cast<usize>(mColumns));
}

auto CreateMapDialog::IsCurrentInputValid(const entt::registry&) const -> bool
{
  return (mTileWidth > 0) && (mTileHeight > 0) && (mRows > 0) && (mColumns > 0);
}

}  // namespace Tactile

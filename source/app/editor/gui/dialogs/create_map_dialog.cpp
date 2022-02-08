#include "create_map_dialog.hpp"

#include <imgui.h>

#include "editor/events/map_events.hpp"
#include "editor/gui/alignment.hpp"
#include "io/persistence/preferences.hpp"
#include "misc/assert.hpp"
#include "tactile.hpp"

namespace tactile {

CreateMapDialog::CreateMapDialog() : dialog_base{"Create New Map"}
{
  set_accept_button_label("Create");
}

void CreateMapDialog::Open()
{
  const auto& prefs = get_preferences();
  mTileWidth = prefs.preferred_tile_width();
  mTileHeight = prefs.preferred_tile_height();

  mRows = 5;
  mColumns = 5;

  make_visible();
}

void CreateMapDialog::on_update(const Model&, entt::dispatcher&)
{
  {
    c_str rowsLabel = "Rows:";
    c_str columnsLabel = "Columns:";
    const auto offset = minimum_offset_to_align(rowsLabel, columnsLabel);

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
    const auto offset = minimum_offset_to_align(tileWidthLabel, tileHeightLabel);

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

void CreateMapDialog::on_accept(entt::dispatcher& dispatcher)
{
  TACTILE_ASSERT(mTileWidth > 0);
  TACTILE_ASSERT(mTileHeight > 0);
  TACTILE_ASSERT(mRows > 0);
  TACTILE_ASSERT(mColumns > 0);
  dispatcher.enqueue<create_map_event>(mTileWidth,
                                       mTileHeight,
                                       static_cast<usize>(mRows),
                                       static_cast<usize>(mColumns));
}

auto CreateMapDialog::is_current_input_valid(const Model&) const -> bool
{
  return (mTileWidth > 0) && (mTileHeight > 0) && (mRows > 0) && (mColumns > 0);
}

}  // namespace tactile

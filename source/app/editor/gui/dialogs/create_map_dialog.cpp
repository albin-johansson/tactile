#include "create_map_dialog.hpp"

#include <imgui.h>

#include "editor/events/map_events.hpp"
#include "editor/gui/alignment.hpp"
#include "io/persistence/preferences.hpp"
#include "misc/assert.hpp"
#include "tactile.hpp"

namespace tactile {

create_map_dialog::create_map_dialog() : dialog_base{"Create New Map"}
{
  set_accept_button_label("Create");
}

void create_map_dialog::show()
{
  const auto& prefs = get_preferences();
  mTileWidth = prefs.preferred_tile_width();
  mTileHeight = prefs.preferred_tile_height();

  mRows = 5;
  mColumns = 5;

  make_visible();
}

void create_map_dialog::on_update(const document_model&, entt::dispatcher&)
{
  {
    const char* rowsLabel = "Rows:";
    const char* columnsLabel = "Columns:";
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
    const char* tileWidthLabel = "Tile width:";
    const char* tileHeightLabel = "Tile height:";
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

void create_map_dialog::on_accept(entt::dispatcher& dispatcher)
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

auto create_map_dialog::is_current_input_valid(const document_model&) const -> bool
{
  return (mTileWidth > 0) && (mTileHeight > 0) && (mRows > 0) && (mColumns > 0);
}

}  // namespace tactile

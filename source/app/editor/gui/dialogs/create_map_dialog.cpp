/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "create_map_dialog.hpp"

#include <imgui.h>

#include "editor/events/map_events.hpp"
#include "editor/gui/alignment.hpp"
#include "io/persistence/preferences.hpp"
#include "misc/assert.hpp"
#include "tactile.hpp"

namespace tactile {

create_map_dialog::create_map_dialog() : ADialog{"Create New Map"}
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

void create_map_dialog::on_update(const DocumentModel&, entt::dispatcher&)
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
  dispatcher.enqueue<CreateMapEvent>(mTileWidth,
                                     mTileHeight,
                                     static_cast<usize>(mRows),
                                     static_cast<usize>(mColumns));
}

auto create_map_dialog::is_current_input_valid(const DocumentModel&) const -> bool
{
  return (mTileWidth > 0) && (mTileHeight > 0) && (mRows > 0) && (mColumns > 0);
}

}  // namespace tactile

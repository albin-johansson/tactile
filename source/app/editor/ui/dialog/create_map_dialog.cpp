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

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/event/map_events.hpp"
#include "editor/lang/language.hpp"
#include "editor/lang/strings.hpp"
#include "editor/ui/alignment.hpp"
#include "io/persist/preferences.hpp"
#include "misc/assert.hpp"

namespace tactile::ui {

CreateMapDialog::CreateMapDialog()
    : ADialog {"Create Map"}
{}

void CreateMapDialog::show()
{
  const auto& prefs = io::get_preferences();

  mTileSize = prefs.preferred_tile_size;
  mRows = 5;
  mColumns = 5;

  const auto& lang = get_current_language();
  set_title(lang.window.create_new_map);
  set_accept_button_label(lang.misc.create);

  make_visible();
}

void CreateMapDialog::on_update(const DocumentModel&, entt::dispatcher&)
{
  const auto& lang = get_current_language();

  {
    const auto& rowsLabel = lang.misc.rows;
    const auto& columnsLabel = lang.misc.columns;
    const auto  offset = minimum_offset_to_align(rowsLabel.c_str(), columnsLabel.c_str());

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(rowsLabel.c_str());
    ImGui::SameLine(offset);
    ImGui::InputInt("##Rows", &mRows);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(columnsLabel.c_str());
    ImGui::SameLine(offset);
    ImGui::InputInt("##Columns", &mColumns);
  }

  ImGui::Separator();

  {
    const auto& tileWidthLabel = lang.misc.tile_width;
    const auto& tileHeightLabel = lang.misc.tile_height;
    const auto  offset =
        minimum_offset_to_align(tileWidthLabel.c_str(), tileHeightLabel.c_str());

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(tileWidthLabel.c_str());
    ImGui::SameLine(offset);
    ImGui::InputInt("##TileWidth", &mTileSize.x);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(tileHeightLabel.c_str());
    ImGui::SameLine(offset);
    ImGui::InputInt("##TileHeight", &mTileSize.y);
  }
}

void CreateMapDialog::on_accept(entt::dispatcher& dispatcher)
{
  TACTILE_ASSERT(mTileSize.x > 0);
  TACTILE_ASSERT(mTileSize.y > 0);
  TACTILE_ASSERT(mRows > 0);
  TACTILE_ASSERT(mColumns > 0);
  dispatcher.enqueue<CreateMapEvent>(mTileSize,
                                     static_cast<usize>(mRows),
                                     static_cast<usize>(mColumns));
}

auto CreateMapDialog::is_current_input_valid(const DocumentModel&) const -> bool
{
  return (mTileSize.x > 0) && (mTileSize.y > 0) && (mRows > 0) && (mColumns > 0);
}

}  // namespace tactile::ui

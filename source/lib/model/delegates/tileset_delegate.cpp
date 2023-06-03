/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "tileset_delegate.hpp"

#include "cmd/tileset/create_tileset.hpp"
#include "cmd/tileset/remove_tileset.hpp"
#include "common/debug/assert.hpp"
#include "model/settings.hpp"
#include "model/systems/document_system.hpp"
#include "ui/dock/tileset/dialogs/create_tileset_dialog.hpp"

namespace tactile {

void on_show_new_tileset_dialog(Model& model, const ShowNewTilesetDialogEvent&)
{
  const auto& settings = model.get<Settings>();

  auto& state = model.get<ui::CreateTilesetDialogState>();
  state.map_entity = sys::get_active_map(model);
  state.image_path.clear();
  state.image_path_preview_buffer.clear();
  state.tile_size = settings.get_preferred_tile_size();
  state.should_open = true;

  TACTILE_ASSERT(state.map_entity != kNullEntity);
}

void on_create_tileset(Model& model, const CreateTilesetEvent& event)
{
  TACTILE_ASSERT(sys::get_active_map(model) == event.map);

  sys::try_execute<cmd::CreateTileset>(model,
                                       event.map,
                                       event.tile_size,
                                       event.image_path);
}

void on_detach_tileset(Model& model, const DetachTilesetEvent& event)
{
  TACTILE_ASSERT(sys::get_active_map(model) == event.map);
  sys::try_execute<cmd::RemoveTileset>(model, event.map, event.attached_tileset);
}

}  // namespace tactile

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

#pragma once

#include "cmd/command.hpp"
#include "common/type/path.hpp"
#include "core/ecs/entity.hpp"
#include "core/math/vector.hpp"
#include "model/registry.hpp"

namespace tactile::cmd {

/// Command for creating a tileset and attaching it to a map document.
class CreateTileset final : public Command {
 public:
  CreateTileset(Registry* registry, Entity map_entity, Int2 tile_size, Path image_path);

  void undo() override;

  void redo() override;

  void dispose() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Registry* mRegistry;
  Entity mMapEntity;
  Int2 mTileSize;
  Path mImagePath;
  Entity mTilesetDocumentEntity {kNullEntity};
  Entity mAttachedTilesetEntity {kNullEntity};
  bool mHasAttachedTileset : 1 {false};

  void _create_tileset_document();
};

}  // namespace tactile::cmd

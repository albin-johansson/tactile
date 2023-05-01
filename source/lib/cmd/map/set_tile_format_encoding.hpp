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

#include "common/type/ecs.hpp"
#include "common/type/maybe.hpp"
#include "core/tile_format.hpp"
#include "cmd/command.hpp"

namespace tactile {

class SetTileFormatEncoding final : public Command {
 public:
  SetTileFormatEncoding(Entity map_entity, TileEncoding encoding);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Entity mMapEntity {kNullEntity};
  TileEncoding mNewEncoding;
  Maybe<TileEncoding> mOldEncoding;
  Maybe<TileCompression> mOldCompression;
};

}  // namespace tactile

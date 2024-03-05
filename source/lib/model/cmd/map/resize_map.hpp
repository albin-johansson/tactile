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

#include "common/type/maybe.hpp"
#include "common/type/ptr.hpp"
#include "core/map.hpp"
#include "core/tile/tile_extent.hpp"
#include "model/cmd/command.hpp"
#include "model/cmd/map/map_command_cache.hpp"
#include "tactile/base/int.hpp"

namespace tactile::cmd {

class ResizeMap final : public Command {
 public:
  ResizeMap(Shared<Map> map, TileExtent extent);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<Map> mMap;
  TileExtent mNewExtent;
  Maybe<TileExtent> mOldExtent {};
  MapCommandCache mCache;

  [[nodiscard]] auto is_lossy_resize() const -> bool;
};

}  // namespace tactile::cmd

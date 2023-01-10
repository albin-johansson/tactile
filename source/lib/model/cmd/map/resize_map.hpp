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

#pragma once

#include "common/numeric.hpp"
#include "common/type/maybe.hpp"
#include "common/type/ptr.hpp"
#include "core/map.hpp"
#include "model/cmd/command.hpp"
#include "model/cmd/map/map_command_cache.hpp"

namespace tactile::cmd {

class ResizeMap final : public Command {
 public:
  ResizeMap(Shared<Map> map, usize n_rows, usize n_cols);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<Map> mMap;
  usize mRows {};
  usize mCols {};
  MapCommandCache mCache;
  Maybe<usize> mPrevRows {};
  Maybe<usize> mPrevCols {};

  [[nodiscard]] auto lossy_resize() const -> bool;
};

}  // namespace tactile::cmd

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

#include <filesystem>  // path

#include "io/maps/ir.hpp"
#include "tactile.hpp"

namespace tactile::emitter {

class emit_info final
{
 public:
  explicit emit_info(std::filesystem::path destination, ir::MapData data);

  [[nodiscard]] auto destination_file() const -> const std::filesystem::path&;

  [[nodiscard]] auto destination_dir() const -> const std::filesystem::path&;

  [[nodiscard]] auto data() const -> const ir::MapData&;

 private:
  std::filesystem::path mDestinationFile;
  std::filesystem::path mDestinationDir;
  ir::MapData mData;
};

}  // namespace tactile::emitter
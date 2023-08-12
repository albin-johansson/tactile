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

#include <filesystem>   // path
#include <string_view>  // string_view

#include "tactile/core/core.hpp"
#include "tactile/core/io/fwd.hpp"

namespace tactile {

class TACTILE_CORE_API IFileFormat {
 public:
  virtual ~IFileFormat() noexcept = default;

  virtual auto load_map(const std::filesystem::path& file, ir::Map* out_map) const
      -> bool = 0;

  virtual auto save_map(const std::filesystem::path& file, const ir::Map& map) const
      -> bool = 0;

  virtual auto load_tileset(const std::filesystem::path& file,
                            ir::Tileset* out_tileset) const -> bool = 0;

  virtual auto save_tileset(const std::filesystem::path& file,
                            const ir::Tileset& tileset) const -> bool = 0;

  [[nodiscard]] virtual auto is_valid_extension(std::string_view extension) const
      -> bool = 0;

  /** Returns a short human-readable name for the file format. */
  [[nodiscard]] virtual auto get_name() const -> std::string_view = 0;
};

}  // namespace tactile

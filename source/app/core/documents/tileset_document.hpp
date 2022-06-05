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

#include <centurion/math.hpp>

#include "core/common/identifiers.hpp"
#include "core/common/math.hpp"
#include "core/common/uuid.hpp"
#include "core/documents/document.hpp"
#include "core/fwd.hpp"

namespace tactile {

/**
 * \brief Represents a tileset (both embedded and external).
 *
 * \details The document model holds a tileset document for each tileset attached to a
 * map. However, tileset documents can be shared across multiple maps as long as they are
 * external.
 *
 * \details Tileset document registries have the following context components:
 * - `CommandStack`
 * - `comp::Context`
 * - `comp::ActiveContext`
 * - `comp::ContextMapping`
 * - `comp::Tileset`
 * - `comp::TilesetCache`
 * - `comp::UvTileSize`
 * - `comp::Texture`
 * - `comp::Viewport`
 *
 * \see `comp::TilesetRef`
 */
class TilesetDocument final : public ADocument
{
 public:
  TilesetDocument(const UUID& id, const comp::Texture& texture, const Vector2i& tileSize);

  void update() override;

  /// Returns the local ID of a tile at a specific position.
  [[nodiscard]] auto tile_at(const TilePos& pos) const -> TileID;

  [[nodiscard]] auto tile_source(TileIndex index) const -> const cen::irect&;

  [[nodiscard]] auto get_displayed_tile(TileIndex index) const -> TileIndex;

  [[nodiscard]] auto info() const -> const comp::Tileset&;

  [[nodiscard]] auto tile_size() const -> Vector2i;

  [[nodiscard]] auto uv_size() const -> Vector2f;

  [[nodiscard]] auto texture() const -> const comp::Texture&;

  [[nodiscard]] auto viewport() const -> const comp::Viewport&;

  [[nodiscard]] auto get_cache() const -> const comp::TilesetCache&;

  [[nodiscard]] auto get_type() const -> DocumentType override
  {
    return DocumentType::Tileset;
  }
};

}  // namespace tactile

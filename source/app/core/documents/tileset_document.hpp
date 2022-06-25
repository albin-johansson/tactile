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
#include "core/common/memory.hpp"
#include "core/common/uuid.hpp"
#include "core/documents/document.hpp"
#include "core/fwd.hpp"
#include "core/tilesets/tileset.hpp"
#include "core/viewport.hpp"

namespace tactile {

/**
 * Represents a loaded tileset.
 *
 * The document model holds a tileset document for each tileset attached to a map.
 * However, tileset documents can be shared across multiple maps as long as they are
 * external.
 */
class TilesetDocument final : public ADocument
{
 public:
  TilesetDocument(const UUID& id, const core::TilesetInfo& info);

  explicit TilesetDocument(const core::TilesetInfo& info);

  void update() override;

  void set_name(std::string name) override;

  [[nodiscard]] auto get_name() const -> const std::string& override;

  /// Returns the local ID of a tile at a specific position.
  [[nodiscard]] auto tile_at(const TilePos& pos) const -> TileIndex;

  [[nodiscard]] auto tile_source(TileIndex index) const -> const cen::irect&;

  [[nodiscard]] auto get_displayed_tile(TileIndex index) const -> TileIndex;

  [[nodiscard]] auto get_viewport() -> core::Viewport& override { return mViewport; }

  [[nodiscard]] auto get_viewport() const -> const core::Viewport& override
  {
    return mViewport;
  }

  [[nodiscard]] auto get_type() const -> DocumentType override
  {
    return DocumentType::Tileset;
  }

  [[nodiscard]] auto get_tileset() -> Shared<core::Tileset> { return mTileset; }

  [[nodiscard]] auto view_tileset() -> core::Tileset& { return *mTileset; }
  [[nodiscard]] auto view_tileset() const -> const core::Tileset& { return *mTileset; }

 private:
  Shared<core::Tileset> mTileset;
  core::Viewport        mViewport;
};

}  // namespace tactile

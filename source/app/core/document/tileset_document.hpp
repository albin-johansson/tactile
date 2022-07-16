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

#include "core/common/memory.hpp"
#include "core/common/uuid.hpp"
#include "core/document/document.hpp"
#include "core/document/document_delegate.hpp"
#include "core/tileset/tileset.hpp"

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
  TilesetDocument(const UUID& id, const TilesetInfo& info);

  explicit TilesetDocument(const TilesetInfo& info);

  void update() override;

  void rename_tileset(std::string name);

  void set_component_index(Shared<ComponentIndex> index) override;

  void set_name(std::string name) override;

  void set_path(fs::path path) override;

  [[nodiscard]] auto has_path() const -> bool override;

  [[nodiscard]] auto get_component_index() -> Shared<ComponentIndex> override;
  [[nodiscard]] auto view_component_index() const -> const ComponentIndex* override;

  [[nodiscard]] auto get_contexts() -> ContextManager& override;
  [[nodiscard]] auto get_contexts() const -> const ContextManager& override;

  [[nodiscard]] auto get_history() -> CommandStack& override;
  [[nodiscard]] auto get_history() const -> const CommandStack& override;

  [[nodiscard]] auto get_viewport() -> Viewport& override;
  [[nodiscard]] auto get_viewport() const -> const Viewport& override;

  [[nodiscard]] auto get_name() const -> const std::string& override;

  [[nodiscard]] auto get_path() const -> const fs::path& override;

  [[nodiscard]] auto get_type() const -> DocumentType override;

  [[nodiscard]] auto get_tileset() -> Shared<Tileset>
  {
    return mTileset;
  }

  [[nodiscard]] auto view_tileset() -> Tileset&
  {
    return *mTileset;
  }

  [[nodiscard]] auto view_tileset() const -> const Tileset&
  {
    return *mTileset;
  }

 private:
  Shared<Tileset>  mTileset;
  DocumentDelegate mDelegate;
};

}  // namespace tactile

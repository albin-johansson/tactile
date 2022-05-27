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
#include <functional>  // function

#include <boost/uuid/uuid_hash.hpp>
#include <entt/fwd.hpp>

#include "core/common/associative.hpp"
#include "core/common/identifiers.hpp"
#include "core/common/ints.hpp"
#include "core/common/math.hpp"
#include "core/common/maybe.hpp"
#include "core/common/memory.hpp"
#include "core/common/uuid.hpp"
#include "core/fwd.hpp"
#include "core/tools/tool_type.hpp"
#include "core/utils/actor.hpp"
#include "editor/document.hpp"
#include "editor/fwd.hpp"

namespace tactile {

/**
 * \brief A facade between the editor and the underlying core documents.
 */
class DocumentModel final
{
  friend class AddTilesetCmd;  // Let this command register tileset documents

 public:
  using VisitorFunc = std::function<void(const UUID&)>;

  /// Updates systems that need to be consistently updated, e.g. animations.
  void update();

  /// Visits each open document.
  void each(const VisitorFunc& func) const;

  /**
   * \brief Creates an empty map document with the specified attributes.
   *
   * \param tileSize the global tile size in the map.
   * \param rows the initial amount of rows in the map.
   * \param columns the initial amount of columns in the map.
   *
   * \return a unique document identifier.
   */
  auto add_map(const Vector2i& tileSize, usize rows, usize columns) -> UUID;

  auto add_tileset(const comp::Texture& texture, const Vector2i& tileSize) -> UUID;

  void attach_tileset_to(const UUID& mapId, const UUID& tilesetId);

  void detach_tileset_from(const UUID& mapId, const UUID& tilesetId);

  void select_document(const UUID& id);

  void remove_document(const UUID& id);

  void set_command_capacity(usize capacity);

  /**
   * \brief Indicates whether there is an open document associated with a file path.
   *
   * \param path the file path that will be checked.
   *
   * \return `true` if there is a document associated with the file path; `false`
   * otherwise.
   */
  [[nodiscard]] auto has_document_with_path(const std::filesystem::path& path) const
      -> bool;

  [[nodiscard]] auto has_active_document() const -> bool;
  [[nodiscard]] auto is_map_active() const -> bool;
  [[nodiscard]] auto is_tileset_active() const -> bool;

  [[nodiscard]] auto active_document() -> ADocument*;
  [[nodiscard]] auto active_document() const -> const ADocument*;

  [[nodiscard]] auto get_active_document() const -> const ADocument&;

  [[nodiscard]] auto active_registry() -> entt::registry*;
  [[nodiscard]] auto active_registry() const -> const entt::registry*;

  [[nodiscard]] auto get_registry(const UUID& documentId) const -> const entt::registry&;

  [[nodiscard]] auto get_active_registry() -> entt::registry&;
  [[nodiscard]] auto get_active_registry() const -> const entt::registry&;

  [[nodiscard]] auto is_map(const UUID& id) const -> bool;
  [[nodiscard]] auto is_tileset(const UUID& id) const -> bool;

  [[nodiscard]] auto get_document(const UUID& id) -> Shared<ADocument>;
  [[nodiscard]] auto get_document(const UUID& id) const -> Shared<const ADocument>;

  [[nodiscard]] auto get_map(const UUID& id) -> Shared<MapDocument>;
  [[nodiscard]] auto get_map(const UUID& id) const -> Shared<const MapDocument>;

  [[nodiscard]] auto get_tileset(const UUID& id) -> Shared<TilesetDocument>;
  [[nodiscard]] auto get_tileset(const UUID& id) const -> Shared<const TilesetDocument>;

  [[nodiscard]] auto active_document_id() const -> Maybe<UUID>;

 private:
  // TODO track open documents
  HashMap<UUID, Shared<ADocument>> mDocuments;  // Holds all documents
  HashMap<UUID, Shared<MapDocument>> mMaps;
  HashMap<UUID, Shared<TilesetDocument>> mTilesets;
  Maybe<UUID> mActiveDocument;

  void register_map(Shared<MapDocument> document);
  void register_tileset(Shared<TilesetDocument> document);
};

}  // namespace tactile

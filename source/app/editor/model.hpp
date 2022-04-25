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
#include <memory>      // unique_ptr
#include <optional>    // optional

#include <boost/container/flat_map.hpp>
#include <entt/fwd.hpp>

#include "core/events/command_events.hpp"
#include "core/events/tool_events.hpp"
#include "editor/document.hpp"
#include "tactile.hpp"

namespace tactile {

/**
 * \brief Represents a facade between the editor and the underlying core documents.
 *
 * \details This class manages all open documents and their respective registries. Other
 * than managing documents, this class provides several utilities for querying the state
 * of the core model from the perspective of the editor (GUI).
 */
class DocumentModel final
{
 public:
  // Documents are stored on the heap to ensure stability, which is important for commands
  using document_map = boost::container::flat_map<MapID, std::unique_ptr<Document>>;
  using const_iterator = document_map::const_iterator;

  /**
   * \brief Updates systems that need to be consistently updated, e.g. animations.
   */
  void update();

  /// \name Document functions
  /// \{

  /**
   * \brief Creates a map based on an existing document representation.
   *
   * \details This function is intended to be used when restoring parsed maps.
   *
   * \param document the document data.
   *
   * \return the unique identifier assigned to the map.
   */
  auto add_map(Document document) -> MapID;

  /**
   * \brief Creates an empty map with the specified attributes.
   *
   * \param tileWidth the global tile width in the map.
   * \param tileHeight the global tile height in the map.
   * \param rows the initial amount of rows in the map.
   * \param columns the initial amount of columns in the map.
   *
   * \return the unique identifier assigned to the map.
   */
  auto add_map(int32 tileWidth, int32 tileHeight, usize rows, usize columns) -> MapID;

  /**
   * \brief Makes a specific map active.
   *
   * \pre the map ID must be valid.
   *
   * \param id the identifier of the map that will be selected.
   */
  void select_map(MapID id);

  /**
   * \brief Closes a currently open map.
   *
   * \pre the map ID must be valid.
   *
   * \details If the specified map is active, this function will try to make the first
   * found open map active, if there is one.
   *
   * \param id the identifier of the map that will be closed.
   */
  void remove_map(MapID id);

  /**
   * \brief Indicates whether a specific map has an associated file path.
   *
   * \pre the map ID must be valid.
   *
   * \param id the identifier of the map that will be queried.
   *
   * \return `true` if the map has a file path; `false` otherwise.
   */
  [[nodiscard]] auto has_path(MapID id) const -> bool;

  /**
   * \brief Returns the (potentially empty) file path associated with a map.
   *
   * \pre the map ID must be valid.
   *
   * \param id the identifier of the map that will be queried.
   *
   * \return the associated file path.
   */
  [[nodiscard]] auto get_path(MapID id) const -> const std::filesystem::path&;

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

  /**
   * \brief Indicates whether there is an active document.
   *
   * \return `true` if there is an active document; `false` otherwise.
   */
  [[nodiscard]] auto has_active_document() const -> bool;

  /**
   * \brief Indicates whether the current document can be saved.
   *
   * \return `true` if the document can be saved; `false` otherwise.
   */
  [[nodiscard]] auto is_save_possible() const -> bool;

  /**
   * \brief Indicates whether the viewport tile size can be decreased in the current
   * document.
   *
   * \return `true` if the viewport tile size can be decreased; `false` otherwise.
   */
  [[nodiscard]] auto can_decrease_viewport_tile_size() const -> bool;

  /**
   * \brief Returns the identifier of the currently active map, if there is one.
   *
   * \return the identifier of the active map.
   */
  [[nodiscard]] auto active_map_id() const -> std::optional<MapID>;

  /**
   * \brief Returns the currently active document, if there is one.
   *
   * \return a (potentially null) pointer to the active document.
   */
  [[nodiscard]] auto active_document() -> Document*;

  /// \copydoc active_document()
  [[nodiscard]] auto active_document() const -> const Document*;

  /**
   * \brief Returns the currently active registry, if there is one.
   *
   * \return a (potentially null) pointer to the active registry.
   */
  [[nodiscard]] auto active_registry() -> entt::registry*;

  /// \copydoc active_registry()
  [[nodiscard]] auto active_registry() const -> const entt::registry*;

  /**
   * \brief Returns the currently active registry.
   *
   * \return a reference to the current registry.
   *
   * \throws TactileError if there is no active registry.
   */
  [[nodiscard]] auto get_active_registry() -> entt::registry&;

  /// \copydoc get_active_registry()
  [[nodiscard]] auto get_active_registry() const -> const entt::registry&;

  /// \} End of document functions

  /// \name Command stack functions
  /// \{

  /**
   * \brief Sets the capacity of the undo stack.
   *
   * \details All open documents will have their command stack capacity updated.
   *
   * \param capacity the new command stack capacity.
   */
  void set_command_capacity(usize capacity);

  /**
   * \brief Indicates whether the current command stack state is "clean".
   *
   * \return `true` if the command stack is clean; `false` otherwise.
   */
  [[nodiscard]] auto is_clean() const -> bool;

  /**
   * \brief Indicates whether the current command stack has an undoable command.
   *
   * \return `true` if there is an undoable command; `false` otherwise.
   */
  [[nodiscard]] auto can_undo() const -> bool;

  /**
   * \brief Indicates whether the current command stack has an redoable command.
   *
   * \return `true` if there is an redoable command; `false` otherwise.
   */
  [[nodiscard]] auto can_redo() const -> bool;

  /**
   * \brief Returns the text associated with the current undoable command.
   *
   * \pre there must be an undoable command.
   *
   * \return the command text.
   */
  [[nodiscard]] auto get_undo_text() const -> const std::string&;

  /**
   * \brief Returns the text associated with the current redoable command.
   *
   * \pre there must be a redoable command.
   *
   * \return the command text.
   */
  [[nodiscard]] auto get_redo_text() const -> const std::string&;

  /// \} End of command stack functions

  /// \name Mouse tool functions
  /// \{

  /**
   * \brief Indicates whether a specific mouse tool is active.
   *
   * \details This function can safely be called without an active registry.
   *
   * \return `true` if the tool is active; `false` otherwise.
   */
  [[nodiscard]] auto is_tool_active(ToolType tool) const -> bool;

  /**
   * \brief Indicates whether a specific mouse tool is usable.
   *
   * \details This function can safely be called without an active registry.
   *
   * \return `true` if the tool is usable; `false` otherwise.
   */
  [[nodiscard]] auto is_tool_possible(ToolType tool) const -> bool;

  /// \} End of mouse tool functions

  [[nodiscard]] auto begin() const noexcept -> const_iterator
  {
    return mDocuments.begin();
  }

  [[nodiscard]] auto end() const noexcept -> const_iterator { return mDocuments.end(); }

 private:
  document_map mDocuments;
  std::optional<MapID> mActiveMap;
  MapID mNextId{1};
};

}  // namespace tactile

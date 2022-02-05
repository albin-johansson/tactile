#pragma once

#include <filesystem>  // path
#include <memory>      // unique_ptr

#include <entt/entt.hpp>  // registry

#include "core/utils/vector_map.hpp"
#include "document.hpp"
#include "events/command_events.hpp"
#include "events/tool_events.hpp"
#include "tactile.hpp"

namespace tactile {

/**
 * \brief Represents a facade between the editor and the underlying core documents.
 *
 * \details This class manages all open documents and their respective registries. Other
 * than managing documents, this class provides several utilities for querying the state
 * of the core model from the perspective of the editor (GUI).
 */
class Model final {
 public:
  // Store documents on the heap to ensure stability, which is important for commands
  using document_map = vector_map<map_id, std::unique_ptr<Document>>;
  using const_iterator = document_map::const_iterator;

  /**
   * \brief Updates systems that need to be consistently updated, e.g. animations.
   */
  void Update();

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
  auto AddMap(Document document) -> map_id;

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
  auto AddMap(int tileWidth, int tileHeight, usize rows, usize columns) -> map_id;

  /**
   * \brief Makes a specific map active.
   *
   * \pre the map ID must be valid.
   *
   * \param id the identifier of the map that will be selected.
   */
  void SelectMap(map_id id);

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
  void RemoveMap(map_id id);

  /**
   * \brief Indicates whether a specific map has an associated file path.
   *
   * \pre the map ID must be valid.
   *
   * \param id the identifier of the map that will be queried.
   *
   * \return `true` if the map has a file path; `false` otherwise.
   */
  [[nodiscard]] auto HasPath(map_id id) const -> bool;

  /**
   * \brief Returns the (potentially empty) file path associated with a map.
   *
   * \pre the map ID must be valid.
   *
   * \param id the identifier of the map that will be queried.
   *
   * \return the associated file path.
   */
  [[nodiscard]] auto GetPath(map_id id) const -> const std::filesystem::path&;

  /**
   * \brief Indicates whether there is an open document associated with a file path.
   *
   * \param path the file path that will be checked.
   *
   * \return `true` if there is a document associated with the file path; `false`
   * otherwise.
   */
  [[nodiscard]] auto HasDocumentWithPath(const std::filesystem::path& path) const -> bool;

  /**
   * \brief Indicates whether there is an active document.
   *
   * \return `true` if there is an active document; `false` otherwise.
   */
  [[nodiscard]] auto HasActiveDocument() const -> bool;

  /**
   * \brief Indicates whether the current document can be saved.
   *
   * \return `true` if the document can be saved; `false` otherwise.
   */
  [[nodiscard]] auto CanSaveDocument() const -> bool;

  /**
   * \brief Indicates whether the viewport tile size can be decreased in the current
   * document.
   *
   * \return `true` if the viewport tile size can be decreased; `false` otherwise.
   */
  [[nodiscard]] auto CanDecreaseViewportTileSize() const -> bool;

  /**
   * \brief Returns the identifier of the currently active map, if there is one.
   *
   * \return the identifier of the active map.
   */
  [[nodiscard]] auto GetActiveMapId() const -> maybe<map_id>;

  /**
   * \brief Returns the currently active document, if there is one.
   *
   * \return a (potentially null) pointer to the active document.
   */
  [[nodiscard]] auto GetActiveDocument() -> Document*;

  /// \copydoc GetActiveDocument()
  [[nodiscard]] auto GetActiveDocument() const -> const Document*;

  /**
   * \brief Returns the currently active registry, if there is one.
   *
   * \return a (potentially null) pointer to the active registry.
   */
  [[nodiscard]] auto GetActiveRegistry() -> entt::registry*;

  /// \copydoc GetActiveRegistry()
  [[nodiscard]] auto GetActiveRegistry() const -> const entt::registry*;

  /**
   * \brief Returns the currently active registry.
   *
   * \return a reference to the current registry.
   *
   * \throws TactileError if there is no active registry.
   */
  [[nodiscard]] auto GetActiveRegistryRef() -> entt::registry&;

  /// \copydoc GetActiveRegistryRef()
  [[nodiscard]] auto GetActiveRegistryRef() const -> const entt::registry&;

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
  void SetCommandCapacity(usize capacity);

  /**
   * \brief Indicates whether the current command stack state is "clean".
   *
   * \return `true` if the command stack is clean; `false` otherwise.
   */
  [[nodiscard]] auto IsClean() const -> bool;

  /**
   * \brief Indicates whether the current command stack has an undoable command.
   *
   * \return `true` if there is an undoable command; `false` otherwise.
   */
  [[nodiscard]] auto CanUndo() const -> bool;

  /**
   * \brief Indicates whether the current command stack has an redoable command.
   *
   * \return `true` if there is an redoable command; `false` otherwise.
   */
  [[nodiscard]] auto CanRedo() const -> bool;

  /**
   * \brief Returns the text associated with the current undoable command.
   *
   * \pre there must be an undoable command.
   *
   * \return the command text.
   */
  [[nodiscard]] auto GetUndoText() const -> const std::string&;

  /**
   * \brief Returns the text associated with the current redoable command.
   *
   * \pre there must be a redoable command.
   *
   * \return the command text.
   */
  [[nodiscard]] auto GetRedoText() const -> const std::string&;

  /// \} End of command stack functions

  /// \name Mouse tool functions
  /// \{

  /**
   * \brief Indicates whether the stamp tool is active.
   *
   * \details This function can safely be called without an active registry.
   *
   * \return `true` if the stamp tool is active; `false` otherwise.
   */
  [[nodiscard]] auto IsStampActive() const -> bool;

  /**
   * \brief Indicates whether the eraser tool is active.
   *
   * \details This function can safely be called without an active registry.
   *
   * \return `true` if the eraser tool is active; `false` otherwise.
   */
  [[nodiscard]] auto IsEraserActive() const -> bool;

  /**
   * \brief Indicates whether the bucket tool is active.
   *
   * \details This function can safely be called without an active registry.
   *
   * \return `true` if the bucket tool is active; `false` otherwise.
   */
  [[nodiscard]] auto IsBucketActive() const -> bool;

  /**
   * \brief Indicates whether the object selection tool is active.
   *
   * \details This function can safely be called without an active registry.
   *
   * \return `true` if the object selection tool is active; `false` otherwise.
   */
  [[nodiscard]] auto IsObjectSelectionActive() const -> bool;

  /**
   * \brief Indicates whether the stamp tool is usable.
   *
   * \details This function can safely be called without an active registry.
   *
   * \return `true` if the stamp tool is usable; `false` otherwise.
   */
  [[nodiscard]] auto IsStampPossible() const -> bool;

  /**
   * \brief Indicates whether the eraser tool is usable.
   *
   * \details This function can safely be called without an active registry.
   *
   * \return `true` if the eraser tool is usable; `false` otherwise.
   */
  [[nodiscard]] auto IsEraserPossible() const -> bool;

  /**
   * \brief Indicates whether the bucket tool is usable.
   *
   * \details This function can safely be called without an active registry.
   *
   * \return `true` if the bucket tool is usable; `false` otherwise.
   */
  [[nodiscard]] auto IsBucketPossible() const -> bool;

  /**
   * \brief Indicates whether the object selection tool is usable.
   *
   * \details This function can safely be called without an active registry.
   *
   * \return `true` if the object selection tool is usable; `false` otherwise.
   */
  [[nodiscard]] auto IsObjectSelectionPossible() const -> bool;

  /// \} End of mouse tool functions

  [[nodiscard]] auto begin() const noexcept -> const_iterator
  {
    return mDocuments.begin();
  }

  [[nodiscard]] auto end() const noexcept -> const_iterator { return mDocuments.end(); }

 private:
  document_map mDocuments;
  maybe<map_id> mActiveMap;
  map_id mNextId{1};
};

}  // namespace tactile

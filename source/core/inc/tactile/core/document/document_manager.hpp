// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstddef>        // size_t
#include <expected>       // expected
#include <memory>         // unique_ptr
#include <system_error>   // error_code
#include <unordered_map>  // unordered_map
#include <vector>         // vector

#include "tactile/base/prelude.hpp"
#include "tactile/core/cmd/command_stack.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

struct MapSpec;
class IDocument;
class IRenderer;

namespace ir {
struct Map;
}  // namespace ir

/**
 * Manages a collection of documents.
 */
class DocumentManager final
{
 public:
  /**
   * Creates a new map document and opens it.
   *
   * \details
   * In addition to being opened, the created document will be made the active
   * document by this function.
   *
   * \param spec The map specification to use.
   *
   * \return
   * The UUID of the map document if successful; an error code otherwise.
   */
  [[nodiscard]]
  auto create_and_open_map(const MapSpec& spec) -> std::expected<UUID, std::error_code>;

  /**
   * Restores a map document from an intermediate map representation.
   *
   * \details
   * This function behaves just like \c create_and_open_map(const MapSpec&).
   *
   * \param renderer The renderer to use for loading textures.
   * \param ir_map   The intermediate map representation.
   *
   * \return
   * The UUID of the map document if successful; an error code otherwise.
   */
  [[nodiscard]]
  auto create_and_open_map(IRenderer& renderer,
                           const ir::Map& ir_map) -> std::expected<UUID, std::error_code>;

  /**
   * Returns the document associated with a given UUID.
   *
   * \param uuid The UUID of the desired document.
   *
   * \return
   * A document.
   */
  [[nodiscard]]
  auto get_document(const UUID& uuid) -> IDocument&;

  /**
   * \copydoc get_document()
   */
  [[nodiscard]]
  auto get_document(const UUID& uuid) const -> const IDocument&;

  /**
   * Returns the currently active document.
   *
   * \return
   * A pointer to the open document; or a null pointer if there is none.
   */
  [[nodiscard]]
  auto get_current_document() -> IDocument*;

  /**
   * \copydoc get_current_document()
   */
  [[nodiscard]]
  auto get_current_document() const -> const IDocument*;

  /**
   * Returns the identifiers of the currently open documents.
   *
   * \return
   * The UUIDs of the open documents.
   */
  [[nodiscard]]
  auto get_open_documents() const -> const std::vector<UUID>&;

  /**
   * Sets the maximum number of commands tracked by each document.
   *
   * \param capacity The new command capacity.
   */
  void set_command_capacity(std::size_t capacity);

  /**
   * Returns the maximum number of commands stored by a document.
   *
   * \return
   * The current command capacity.
   */
  [[nodiscard]]
  auto command_capacity() const -> std::size_t;

  /**
   * Returns the command history associated with a given document.
   *
   * \param uuid The associated document identifier.
   *
   * \return
   * The command stack associated with the document.
   */
  [[nodiscard]]
  auto get_history(const UUID& uuid) -> CommandStack&;

  /**
   * \copydoc get_history()
   */
  [[nodiscard]]
  auto get_history(const UUID& uuid) const -> const CommandStack&;

  /**
   * Indicates whether a map document is currently active.
   *
   * \return
   * True if a map document is active; false otherwise.
   */
  [[nodiscard]]
  auto is_map_active() const -> bool;

 private:
  std::unordered_map<UUID, std::unique_ptr<IDocument>> mDocuments {};
  std::vector<UUID> mOpenDocuments {};
  UUID mActiveDocument {};
  std::unordered_map<UUID, CommandStack> mHistories {};
  std::size_t mCommandCapacity {100};
};

}  // namespace tactile

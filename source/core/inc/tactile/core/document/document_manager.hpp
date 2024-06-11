// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/hash_map.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/container/vector.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/cmd/command_stack.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

struct MapSpec;
class IDocument;

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
  auto create_and_open_map(const MapSpec& spec) -> Result<UUID>;

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
  auto get_open_documents() const -> const Vector<UUID>&;

  /**
   * Sets the maximum number of commands tracked by each document.
   *
   * \param capacity The new command capacity.
   */
  void set_command_capacity(usize capacity);

  /**
   * Returns the maximum number of commands stored by a document.
   *
   * \return
   * The current command capacity.
   */
  [[nodiscard]]
  auto command_capacity() const -> usize;

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
  HashMap<UUID, Unique<IDocument>> mDocuments {};
  Vector<UUID> mOpenDocuments {};
  UUID mActiveDocument {};
  HashMap<UUID, CommandStack> mHistories {};
  usize mCommandCapacity {100};
};

}  // namespace tactile

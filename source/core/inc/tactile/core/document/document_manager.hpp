// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/hash_map.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/container/vector.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

struct MapSpec;
class IDocument;

/// \addtogroup Document
/// \{

/**
 * Manages a collection of documents.
 */
class DocumentManager final
{
 public:
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
};

/// \}

}  // namespace tactile

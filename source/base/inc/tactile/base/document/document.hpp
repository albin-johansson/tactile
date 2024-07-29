// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

class IDocumentVisitor;
class UUID;
class Registry;  // NB: This type is intentionally defined in core.

/**
 * Provides the basic document API.
 */
class IDocument
{
 public:
  TACTILE_INTERFACE_CLASS(IDocument);

  /**
   * Inspects the document.
   *
   * \param visitor The visitor to use.
   *
   * \return
   * Nothing if successful; an error code otherwise.
   */
  [[nodiscard]]
  virtual auto accept(IDocumentVisitor& visitor) const -> Result<void> = 0;

  /**
   * Updates the state of the document.
   *
   * \details
   * This function should be called once per frame in order to ensure correct
   * handling of time-sensitive aspects, such as animations.
   */
  virtual void update() = 0;

  /**
   * Sets the associated save file path.
   *
   * \note
   * Documents have no file path associated with them by default.
   *
   * \param path A save file path.
   */
  virtual void set_path(Path path) = 0;

  /**
   * Returns the associated save file path.
   *
   * \return
   * The associated file path, if any.
   */
  [[nodiscard]]
  virtual auto get_path() const -> const Path* = 0;

  /**
   * Returns the associated entity registry.
   *
   * \return
   * A registry.
   */
  [[nodiscard]]
  virtual auto get_registry() -> Registry& = 0;

  /**
   * \copydoc get_registry()
   */
  [[nodiscard]]
  virtual auto get_registry() const -> const Registry& = 0;

  /**
   * Returns the graphical size of the associated content.
   *
   * \return
   * The content size.
   */
  [[nodiscard]]
  virtual auto get_content_size() const -> Float2 = 0;

  /**
   * Returns the logical size of tiles in the document.
   *
   * \return
   * A logical tile size.
   */
  [[nodiscard]]
  virtual auto get_tile_size() const -> Int2 = 0;

  /**
   * Returns the UUID associated with the document.
   *
   * \return
   * The associated UUID.
   */
  [[nodiscard]]
  virtual auto get_uuid() const -> const UUID& = 0;
};

}  // namespace tactile

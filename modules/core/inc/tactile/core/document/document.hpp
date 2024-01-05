// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

class ComponentSet;
class DocumentViewport;

/**
 * Provides the common document API.
 */
class IDocument {
 public:
  TACTILE_INTERFACE_CLASS(IDocument);

  /**
   * Updates the state of the document.
   *
   * \details
   *    This function should be called once per frame, to enable correct handling
   *    of time-sensitive aspects, such as animations.
   */
  virtual void update() = 0;

  /**
   * Sets the associated save file path.
   *
   * \note
   *    This property is initially not set.
   *
   * \param path The save file path.
   */
  virtual void set_path(FilePath path) = 0;

  /**
   * Returns the associated save file path.
   *
   * \return
   *    The associated file path, if any.
   */
  [[nodiscard]]
  virtual auto path() const -> const FilePath* = 0;

  /**
   * Returns the associated component set.
   *
   * \return
   *    A component set.
   */
  [[nodiscard]]
  virtual auto component_set() -> ComponentSet& = 0;

  /** \copydoc component_set() */
  [[nodiscard]]
  virtual auto component_set() const -> const ComponentSet& = 0;

  /**
   * Returns the viewport associated with the document.
   *
   * \return
   *    A viewport.
   */
  [[nodiscard]]
  virtual auto viewport() -> DocumentViewport& = 0;

  /** \copydoc viewport() */
  [[nodiscard]]
  virtual auto viewport() const -> const DocumentViewport& = 0;
};

}  // namespace tactile
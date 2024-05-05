// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/path.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

class Registry;

/// \addtogroup Document
/// \{

/**
 * Provides the basic document API.
 */
class IDocument
{
 public:
  TACTILE_INTERFACE_CLASS(IDocument);

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
   * Returns the "root" entity identifier.
   *
   * \return
   * An entity identifier.
   */
  [[nodiscard]]
  virtual auto get_root_entity() const -> EntityID = 0;
};

/// \}

}  // namespace tactile

// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

class DocumentViewport;

/**
 * \interface IDocument
 * \brief Provides the common document API.
 */
class IDocument {
 public:
  TACTILE_INTERFACE_CLASS(IDocument);

  /**
   * \brief Updates the state of the document.
   *
   * \details This function should be called once per frame, to enable correct handling
   *          of time-sensitive aspects, such as animations.
   */
  virtual void update() = 0;

  /**
   * \brief Sets the associated save file path.
   *
   * \note This property is initially not set.
   *
   * \param path the save file path.
   */
  virtual void set_path(FilePath path) = 0;

  /**
   * \brief Returns the associated save file path, if any.
   *
   * \return a file path.
   */
  [[nodiscard]]
  virtual auto get_path() const -> const Maybe<FilePath>& = 0;


  /**
   * \brief Returns the viewport associated with the document.
   *
   * \return a viewport.
   */
  [[nodiscard]]
  virtual auto viewport() -> DocumentViewport& = 0;

  /** \copydoc viewport() */
  [[nodiscard]]
  virtual auto viewport() const -> const DocumentViewport& = 0;
};

}  // namespace tactile
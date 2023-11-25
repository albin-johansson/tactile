// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/functional/maybe.hpp"
#include "tactile/core/io/filesystem.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \interface IDocument
 * \brief Provides the common document API.
 */
class TACTILE_CORE_API IDocument {
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
  virtual auto get_path() const -> Maybe<FilePath> = 0;
};

}  // namespace tactile
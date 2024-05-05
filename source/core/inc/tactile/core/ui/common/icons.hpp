// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile::ui {

/// \addtogroup UI
/// \{

/**
 * Represents the different icons used in the UI.
 */
enum class Icon
{
  kCreate,
  kRemove,
  kDuplicate,
  kMoveUp,
  kMoveDown,
  kMoveLeft,
  kMoveRight,
  kAlignCenter,
  kZoomIn,
  kZoomOut,
  kZoomReset,
  kGrid,
  kBug,
  kToolbox,
  kBrush,
};

/**
 * Returns an icon string associated with a given icon identifier.
 *
 * \param icon The icon to query.
 *
 * \return
 * An icon string. An empty string is returned if the icon identifier isn't
 * recognized.
 */
[[nodiscard]]
auto to_string(Icon icon) noexcept -> const char*;

/**
 * Returns an icon string for a given icon identifier for use before labels.
 *
 * \details
 * This result of this function is equivalent to \c to_string(Icon) with an
 * appended \c " " string.
 *
 * \param icon The icon to query.
 *
 * \return
 * An icon prefix string. An empty string is returned if the icon identifier
 * isn't recognized.
 */
[[nodiscard]]
auto to_prefix_string(Icon icon) noexcept -> const char*;

/// \}

}  // namespace tactile::ui

// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/map/map_spec.hpp"

namespace tactile {

/// \addtogroup Event
/// \{

/**
 * Event for opening the map document creation dialog.
 */
struct ShowNewMapDialogEvent final
{};

/**
 * Event for opening the map document loading dialog.
 */
struct ShowOpenMapDialogEvent final
{};

/**
 * Event for creating new map documents.
 */
struct CreateMapEvent final
{
  /** The requested map specification. */
  MapSpec spec;
};

/// \}

}  // namespace tactile

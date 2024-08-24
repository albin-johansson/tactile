// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Provides identifiers for save format implementations.
 *
 * \note
 * The enumerator values must be kept in sync with the associated Protobuf enum.
 */
enum class SaveFormatId : uint8
{
  /** The native YAML save format. */
  kTactileYaml = 0,

  /** The Tiled JSON (aka TMJ) save format. */
  kTiledTmj = 1,

  /** The Tiled XML (aka TMX/TSX) save format. */
  kTiledTmx = 2,

  /** The Godot scene (aka TSCN/ESCN) save format. */
  kGodotTscn = 3,
};

}  // namespace tactile

// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Provides identifiers for different save format implementations.
 */
enum class SaveFormatId : int8 {
  kUnknown,      ///< An unknown save format.
  kTactileYaml,  ///< The native Tactile YAML save format.
  kTiledJson,    ///< The Tiled JSON (TMJ) save format.
  kTiledXml,     ///< The Tiled XML (TMX/TSX) save format.
  kGodotScene    ///< The Godot scene (TSCN/ESCN) format.
};

/**
 * \brief Returns a human-readable identifier for a given save format.
 *
 * \param save_format_id a save format identifier.
 *
 * \return a save format name.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto to_string(SaveFormatId save_format_id) -> StringView;

}  // namespace tactile

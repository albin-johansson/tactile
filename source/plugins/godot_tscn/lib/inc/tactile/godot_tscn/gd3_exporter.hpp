// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>  // expected

#include "tactile/base/debug/error_code.hpp"
#include "tactile/base/io/save/save_format.hpp"
#include "tactile/godot_tscn/api.hpp"

namespace tactile::godot_tscn {

struct Gd3Map;

/**
 * Saves a Godot 3 scene.
 *
 * \param map     The Godot 3 map to save.
 * \param options The save options to use.
 *
 * \return
 * Nothing if successful; an error code otherwise.
 */
[[nodiscard]]
TACTILE_GODOT_API auto save_godot3_scene(const Gd3Map& map,
                                         const SaveFormatWriteOptions& options)
    -> std::expected<void, ErrorCode>;

}  // namespace tactile::godot_tscn

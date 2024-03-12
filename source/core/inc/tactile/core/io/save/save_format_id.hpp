// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/path.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/int.hpp"

namespace tactile {

/**
 * Provides identifiers for save format implementations.
 *
 * \note
 *    The enumerator values must be kept in sync with the
 *    associated Protobuf enum.
 */
enum class SaveFormatId : uint8 {
  kTactileYaml = 0,  ///< The native YAML save format.
  kTiledJson = 1,    ///< The Tiled JSON (aka TMJ) save format.
  kTiledXml = 2,     ///< The Tiled XML (aka TMX/TSX) save format.
  kGodotScene = 3,   ///< The Godot scene (aka TSCN/ESCN) save format.
};

/**
 * Returns a language-agnostic identifier for a save format identifier.
 *
 * \details
 *    The returned string is guaranteed to be null-terminated.
 *
 * \param format The save format to query.
 *
 * \return
 *    A string identifier.
 */
[[nodiscard]]
auto to_string(SaveFormatId format) -> StringView;

/**
 * Returns a valid file extension for a given save format.
 *
 * \note
 *    Several file extensions are valid for most formats. For example, \c ".yml" and
 *    \c ".yaml" for the Tactile YAML format.
 *
 * \param format The save format to consider.
 *
 * \return
 *    A file extension string, on the form \c ".foo".
 */
[[nodiscard]]
auto get_file_extension(SaveFormatId format) -> String;

/**
 * Indicates whether a path features a valid Tactile YAML file extension.
 *
 * \param path The file path to check.
 *
 * \return
 *    True if the file extension is valid; false otherwise.
 */
[[nodiscard]]
auto has_valid_tactile_yaml_extension(const Path& path) -> bool;

/**
 * Indicates whether a path features a valid Tiled JSON file extension.
 *
 * \param path The file path to check.
 *
 * \return
 *    True if the file extension is valid; false otherwise.
 */
[[nodiscard]]
auto has_valid_tiled_json_extension(const Path& path) -> bool;

/**
 * Indicates whether a path features a valid Tiled XML file extension.
 *
 * \param path The file path to check.
 *
 * \return
 *    True if the file extension is valid; false otherwise.
 */
[[nodiscard]]
auto has_valid_tiled_xml_extension(const Path& path) -> bool;

}  // namespace tactile

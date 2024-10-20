// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>    // expected
#include <filesystem>  // path
#include <string>      // string

#include "tactile/base/container/string_map.hpp"
#include "tactile/base/debug/error_code.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile::core {

using IniSection = StringMap<std::string>;
using IniData = StringMap<IniSection>;

/**
 * Parses an INI file.
 *
 * \details
 * There's no standard INI format, so this function uses the following
 * conventions.
 * - Lines starting with a ';' character are treated as comments, i.e.,
 *   ignored.\n
 * - Comments cannot be added at the end of lines. \n
 * - Every key-value pair must be a child of a named section. \n
 * - Section names cannot be empty. \n
 * - Keys cannot be empty. \n
 * - Whitespace around keys and values is ignored. \n
 * - Keys cannot contain '=' characters.
 *
 * \details
 * The following is an example of an INI file that this function considers
 * valid.
 * \code{.ini}
 * ; This is a comment.
 * [A]
 * a = 12
 * b = 34
 *
 * ; This is another comment.
 * [B]
 * a = hello world
 * z = foobar
 * \endcode
 *
 * \param path The file path to an INI file.
 *
 * \return
 * The parsed INI data if successful; an error code otherwise.
 *
 * \see https://en.m.wikipedia.org/wiki/INI_file
 */
[[nodiscard]]
auto parse_ini(const std::filesystem::path& path) -> std::expected<IniData, ErrorCode>;

}  // namespace tactile::core

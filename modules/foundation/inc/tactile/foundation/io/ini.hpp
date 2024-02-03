// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/container/string_map.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

using IniSection = StringMap<String>;
using IniData = StringMap<IniSection>;

/**
 * Parses an INI file.
 *
 * \details
 *    There's no standard INI format, so this function uses the following
 *    conventions.
 *      - Lines starting with a ';' character are treated as comments, i.e., ignored.
 *      - Comments cannot be added at the end of lines.
 *      - Every key-value pair must be a child of a named section.
 *      - Section names cannot be empty.
 *      - Keys cannot be empty.
 *      - Whitespace around keys and values is ignored.
 *      - Keys cannot contain '=' characters.
 *
 * \details
 *    The following is an example of an INI file that this function considers valid.
 *    \code{.ini}
 *    ; This is a comment.
 *    [A]
 *    a = 12
 *    b = 34
 *
 *    ; This is another comment.
 *    [B]
 *    a = hello world
 *    z = foobar
 *    \endcode
 *
 * \param path The file path to the INI file to parse.
 *
 * \return
 *    The parsed INI data.
 *
 * \see https://en.m.wikipedia.org/wiki/INI_file
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto parse_ini(const FilePath& path) -> Result<IniData>;

}  // namespace tactile

// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/container/string_map.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

using IniSection = StringMap<String>;
using IniData = StringMap<IniSection>;

/**
 * Parses an INI file.
 *
 * \details
 *    There's no standard INI format, so this function uses the following
 *    conventions.
 *      - Lines starting with a ';' character are treated as comments, i.e., ignored.\n
 *      - Comments cannot be added at the end of lines.                              \n
 *      - Every key-value pair must be a child of a named section.                   \n
 *      - Section names cannot be empty.                                             \n
 *      - Keys cannot be empty.                                                      \n
 *      - Whitespace around keys and values is ignored.                              \n
 *      - Keys cannot contain '=' characters.                                        \n
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
 * \param path The file path to an INI file.
 *
 * \return
 *    The parsed INI data if successful; an error code otherwise.
 *
 * \see https://en.m.wikipedia.org/wiki/INI_file
 */
[[nodiscard]]
auto parse_ini(const Path& path) -> Result<IniData>;

}  // namespace tactile

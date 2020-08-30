#pragma once

#include <QPalette>
#include <QString>

#include "tactile_types.hpp"

namespace tactile::json {

/**
 * @brief Parses a palette from a JSON file.
 *
 * @details This function will throw if the palette couldn't be parsed.
 *
 * @param file the path to the JSON file that represents the palette.
 *
 * @return a palette that was parsed from the JSON file.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto parse_palette(const QString& file) -> QPalette;

}  // namespace tactile::json

#pragma once

#include <QPalette>  // QPalette
#include <QString>   // QString

#include "czstring.hpp"
#include "maybe.hpp"

namespace tactile {

/**
 * \brief Parses a palette from a JSON file.
 *
 * \param file the path to the JSON file that represents the palette.
 *
 * \return a palette that was parsed from the JSON file; `std::nullopt` if
 * something goes wrong.
 *
 * \since 0.1.0
 */
[[nodiscard]] auto parse_palette(const QString& file) -> maybe<QPalette>;

}  // namespace tactile

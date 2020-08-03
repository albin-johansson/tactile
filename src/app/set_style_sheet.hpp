#pragma once

#include <QApplication>

#include "tactile_types.hpp"

namespace tactile {

/**
 * @brief Loads and applies a stylesheet to the application.
 *
 * @param file the file path of the QSS file.
 *
 * @since 0.1.0
 */
void set_style_sheet(QApplication& app, czstring file);

}  // namespace tactile

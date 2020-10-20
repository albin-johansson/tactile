#pragma once

#include <QApplication>
#include <QString>

#include "czstring.hpp"

namespace tactile {

/**
 * @brief Loads and applies a stylesheet to the application.
 *
 * @param app the application that will consume the stylesheet.
 * @param file the file path of the QSS file.
 *
 * @since 0.1.0
 */
void set_style_sheet(QApplication& app, const QString& file);

}  // namespace tactile

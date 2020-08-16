#pragma once

#include <QDialog>

#include "basic_widget_macro.hpp"

namespace Ui {

class AboutUI;

}

namespace tactile::ui {

/**
 * @class about_dialog
 *
 * @brief Represents the "About Tactile" dialog window that displays
 * information about the application.
 *
 * @see `QDialog`
 *
 * @since 0.1.0
 *
 * @headerfile about_dialog.hpp
 */
TACTILE_BASIC_WIDGET_DECL(about_dialog, QDialog, Ui::AboutUI)

}  // namespace tactile::gui

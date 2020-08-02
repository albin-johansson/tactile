#pragma once
#include <QMargins>

#include "smart_pointers.hpp"

class QDockWidget;
class QWidget;

namespace tactile {

/**
 * Creates and returns a unique pointer to a dock widget to be used by the
 * main window.
 *
 * @param widget a pointer to the widget that will be contained in the dock
 * widget.
 * @param name the name of the dock widget object.
 * @param margins the margins of the dock widget, defaults to (0,0,0,0).
 * @param areas the allowed areas of the dock widget, defaults to left and
 * right.
 * @return a unique pointer to a dock widget.
 * @throws BadArg if any supplied pointers are null.
 * @since 0.1.0
 */
[[nodiscard]] Unique<QDockWidget> create_dock_widget(
    QWidget* widget,
    const char* name,
    const QMargins& margins = {0, 0, 0, 0},
    QFlags<Qt::DockWidgetArea> areas = {
        Qt::DockWidgetArea::LeftDockWidgetArea,
        Qt::DockWidgetArea::RightDockWidgetArea});

}  // namespace tactile

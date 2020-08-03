#pragma once

#include <QMargins>
#include <memory>

#include "tactile_types.hpp"

class QDockWidget;
class QWidget;

namespace tactile::ui {

[[nodiscard]] constexpr auto default_dock_widget_areas() noexcept
    -> QFlags<Qt::DockWidgetArea>
{
  return {Qt::DockWidgetArea::LeftDockWidgetArea,
          Qt::DockWidgetArea::RightDockWidgetArea};
}

/**
 * @brief Creates and returns a unique pointer to a dock widget to be used by
 * the main window.
 *
 * @param widget a pointer to the widget that will be contained in the dock
 * widget.
 * @param name the name of the dock widget object.
 * @param margins the margins of the dock widget, defaults to (0,0,0,0).
 * @param areas the allowed areas of the dock widget, defaults to left and
 * right.
 *
 * @return a unique pointer to a dock widget.
 *
 * @throws tactile_error if any supplied pointers are null.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto create_dock_widget(
    QWidget* widget,
    czstring name,
    const QMargins& margins = {},
    QFlags<Qt::DockWidgetArea> areas = default_dock_widget_areas())
    -> std::unique_ptr<QDockWidget>;

}  // namespace tactile::ui

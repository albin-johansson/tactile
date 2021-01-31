#include "dock_widget.hpp"

namespace tactile::gui {

dock_widget::dock_widget(QWidget* parent) : QDockWidget{parent}
{}

void dock_widget::closeEvent(QCloseEvent* event)
{
  QDockWidget::closeEvent(event);
  emit closed();
}

}  // namespace tactile::gui

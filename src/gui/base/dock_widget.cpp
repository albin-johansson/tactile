#include "dock_widget.hpp"

namespace tactile {

DockWidget::DockWidget(QWidget* parent) : QDockWidget{parent}
{}

void DockWidget::closeEvent(QCloseEvent* event)
{
  QDockWidget::closeEvent(event);
  emit S_Closed();
}

}  // namespace tactile

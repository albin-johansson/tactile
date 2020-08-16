#include "create_dock_widget.hpp"

#include <QDockWidget>
#include <QLayout>

#include "tactile_error.hpp"

namespace tactile::ui {

auto create_dock_widget(QWidget* widget,
                        czstring name,
                        const QMargins& margins,
                        QFlags<Qt::DockWidgetArea> areas) -> owner<QDockWidget*>
{
  if (!widget || !name) {
    throw tactile_error{"Failed to create dock widget!"};
  }

  auto dock = new QDockWidget{};
  dock->setObjectName(name);
  dock->setVisible(false);
  dock->setAllowedAreas(areas);
  dock->setWidget(widget);
  dock->widget()->layout()->setContentsMargins(margins);
  return dock;
}

}  // namespace tactile::gui

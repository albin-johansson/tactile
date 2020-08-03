#include "create_dock_widget.hpp"

#include <QDockWidget>
#include <QLayout>

#include "tactile_error.hpp"
#include "tactile_types.hpp"

namespace tactile::ui {

auto create_dock_widget(QWidget* widget,
                        czstring name,
                        const QMargins& margins,
                        QFlags<Qt::DockWidgetArea> areas)
    -> std::unique_ptr<QDockWidget>
{
  if (!widget || !name) {
    throw tactile_error{"Failed to create dock widget!"};
  }

  auto dock = std::make_unique<QDockWidget>();
  dock->setObjectName(name);
  dock->setVisible(false);
  dock->setAllowedAreas(areas);
  dock->setWidget(widget);
  dock->widget()->layout()->setContentsMargins(margins);
  return dock;
}

}  // namespace tactile::ui

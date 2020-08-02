#include "create_dock_widget.hpp"

#include <QDockWidget>
#include <QLayout>

#include "tactile_types.hpp"

namespace tactile {

[[nodiscard]] Unique<QDockWidget> create_dock_widget(
    QWidget* widget,
    const char* name,
    const QMargins& margins,
    QFlags<Qt::DockWidgetArea> areas)
{
  if (!widget || !name) {
    throw BadArg{"Failed to create dock widget!"};
  }

  auto dock = std::make_unique<QDockWidget>();
  dock->setObjectName(name);
  dock->setVisible(false);
  dock->setAllowedAreas(areas);
  dock->setWidget(widget);
  dock->widget()->layout()->setContentsMargins(margins);
  return dock;
}

}  // namespace tactile

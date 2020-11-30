#include "properties_dock.hpp"

namespace tactile::gui {

properties_dock::properties_dock(QWidget* parent) : dock_widget{parent}
{
  setObjectName(QStringLiteral(u"properties_dock"));
  setWindowTitle(tr("Properties"));
}

}  // namespace tactile::gui

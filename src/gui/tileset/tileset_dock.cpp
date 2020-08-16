#include "tileset_dock.hpp"

namespace tactile::gui {

tileset_dock::tileset_dock(QWidget* parent)
    : QDockWidget{parent}, m_widget{new tileset_widget{this}}
{
  setObjectName("tilesetDock");
  setWindowTitle(tr("Tilesets"));
  setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  setWidget(m_widget);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  connect(m_widget,
          &tileset_widget::request_new_tileset,
          this,
          &tileset_dock::new_tileset_requested);
}

}  // namespace tactile::gui

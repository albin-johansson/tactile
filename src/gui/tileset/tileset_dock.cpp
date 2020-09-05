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

  connect(m_widget,
          &tileset_widget::selected_tileset,
          this,
          &tileset_dock::selected_tileset);

  connect(m_widget,
          &tileset_widget::removed_tileset,
          this,
          &tileset_dock::removed_tileset);

  connect(m_widget,
          &tileset_widget::tileset_selection_changed,
          this,
          &tileset_dock::tileset_selection_changed);
}

}  // namespace tactile::gui

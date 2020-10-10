#include "tileset_dock.hpp"

namespace tactile::gui {

tileset_dock::tileset_dock(QWidget* parent)
    : QDockWidget{parent},
      m_widget{new tileset_widget{this}}
{
  setObjectName("tilesetDock");
  setWindowTitle(tr("Tilesets"));
  setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  setWidget(m_widget);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  connect(m_widget,
          &tileset_widget::ui_requested_tileset,
          this,
          &tileset_dock::ui_requested_tileset);

  connect(m_widget,
          &tileset_widget::ui_selected_tileset,
          this,
          &tileset_dock::ui_selected_tileset);

  connect(m_widget,
          &tileset_widget::ui_removed_tileset,
          this,
          &tileset_dock::ui_removed_tileset);

  connect(m_widget,
          &tileset_widget::tileset_selection_changed,
          this,
          &tileset_dock::tileset_selection_changed);
}

}  // namespace tactile::gui

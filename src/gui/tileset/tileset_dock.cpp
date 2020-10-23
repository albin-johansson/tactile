#include "tileset_dock.hpp"

namespace tactile::gui {

tileset_dock::tileset_dock(QWidget* parent)
    : QDockWidget{parent},
      m_widget{new tileset_widget{this}}
{
  setObjectName(QStringLiteral(u"tileset_dock"));
  setWindowTitle(tr("Tilesets"));
  setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setWidget(m_widget);

  using widget = tileset_widget;
  using dock = tileset_dock;
  // clang-format off
  connect(m_widget, &widget::ui_add_tileset, this, &dock::ui_add_tileset);
  connect(m_widget, &widget::ui_select_tileset, this, &dock::ui_select_tileset);
  connect(m_widget, &widget::ui_remove_tileset, this, &dock::ui_remove_tileset);
  connect(m_widget, &widget::ui_set_tileset_selection, this, &dock::ui_set_tileset_selection);
  // clang-format on
}

}  // namespace tactile::gui

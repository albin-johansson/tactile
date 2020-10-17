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
  connect(m_widget, &widget::ui_requested_tileset, this, &dock::ui_requested_tileset);
  connect(m_widget, &widget::ui_selected_tileset, this, &dock::ui_selected_tileset);
  connect(m_widget, &widget::ui_removed_tileset, this, &dock::ui_removed_tileset);
  connect(m_widget, &widget::tileset_selection_changed, this, &dock::tileset_selection_changed);
  // clang-format on
}

}  // namespace tactile::gui

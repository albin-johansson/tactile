#include "tileset_dock.hpp"

#include "tactile_qstring.hpp"
#include "tileset_widget.hpp"

namespace tactile::gui {

tileset_dock::tileset_dock(QWidget* parent)
    : dock_widget{parent}
    , m_widget{new tileset_widget{this}}
{
  setObjectName(TACTILE_QSTRING(u"tileset_dock"));
  setWindowTitle(tr("Tilesets"));
  setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setContentsMargins(0, 0, 0, 0);
  setWidget(m_widget);

  using widget = tileset_widget;
  using dock = tileset_dock;
  // clang-format off
  connect(m_widget, &widget::ui_add_tileset, this, &dock::ui_add_tileset);
  connect(m_widget, &widget::ui_select_tileset, this, &dock::ui_select_tileset);
  connect(m_widget, &widget::ui_remove_tileset, this, &dock::ui_remove_tileset);
  connect(m_widget, &widget::ui_rename_tileset, this, &dock::ui_rename_tileset);
  connect(m_widget, &widget::ui_set_tileset_selection, this, &dock::ui_set_tileset_selection);
  // clang-format on
}

void tileset_dock::switched_map(map_id id)
{
  m_widget->selected_map(id);
}

void tileset_dock::added_tileset(const map_id map,
                                 const tileset_id id,
                                 const core::tileset& tileset)
{
  m_widget->added_tileset(map, id, tileset);
}

void tileset_dock::removed_tileset(const tileset_id id)
{
  m_widget->removed_tileset(id);
}

}  // namespace tactile::gui

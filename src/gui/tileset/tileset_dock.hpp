#pragma once

#include <QDockWidget>

#include "position.hpp"
#include "tileset.hpp"
#include "tileset_widget.hpp"

namespace tactile::gui {

class tileset_dock final : public QDockWidget
{
  Q_OBJECT

 public:
  explicit tileset_dock(QWidget* parent = nullptr);

 signals:
  void ui_add_tileset();
  void ui_select_tileset(tileset_id id);
  void ui_remove_tileset(tileset_id id);
  void ui_set_tileset_selection(const core::tileset::selection& selection);

 public slots:
  void selected_map(map_id map)
  {
    m_widget->selected_map(map);
  }

  void added_tileset(map_id map, tileset_id id, const core::tileset& tileset)
  {
    m_widget->added_tileset(map, id, tileset);
  }

  void removed_tileset(tileset_id id)
  {
    m_widget->removed_tileset(id);
  }

 private:
  tileset_widget* m_widget{};
};

}  // namespace tactile::gui

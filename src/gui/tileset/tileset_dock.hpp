#pragma once

#include <qdockwidget.h>

#include "position.hpp"
#include "tileset_widget.hpp"

namespace tactile::gui {

class tileset_dock final : public QDockWidget
{
  Q_OBJECT

 public:
  explicit tileset_dock(QWidget* parent = nullptr);

  void add_tileset(map_id map, tileset_id id, const core::tileset& tileset)
  {
    m_widget->add_tileset(map, id, tileset);
  }

 signals:
  void ui_requested_tileset();

  void ui_selected_tileset(tileset_id id);

  void ui_removed_tileset(tileset_id id);

  void tileset_selection_changed(core::position topLeft,
                                 core::position bottomRight);

 public slots:
  void select_map(map_id map)
  {
    m_widget->selected_map(map);
  }

  void remove_tileset(tileset_id id)
  {
    m_widget->remove_tileset(id);
  }

 private:
  tileset_widget* m_widget{};
};

}  // namespace tactile::gui

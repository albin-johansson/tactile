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

  [[nodiscard]] auto get_tileset_widget() noexcept -> tileset_widget*
  {
    return m_widget;
  }

 signals:
  void new_tileset_requested();

  void selected_tileset(tileset_id id);

  void removed_tileset(tileset_id id);

  void tileset_selection_changed(core::position topLeft,
                                 core::position bottomRight);

 public slots:
  void selected_map(map_id map)
  {
    m_widget->selected_map(map);
  }

 private:
  tileset_widget* m_widget{};
};

}  // namespace tactile::gui

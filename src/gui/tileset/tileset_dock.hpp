#pragma once

#include <QDockWidget>

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

 private:
  tileset_widget* m_widget{};
};

}  // namespace tactile::gui

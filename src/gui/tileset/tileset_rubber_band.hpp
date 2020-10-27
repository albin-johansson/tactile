#pragma once

#include <QRubberBand>

#include "tile_height.hpp"
#include "tile_width.hpp"
#include "tileset.hpp"

namespace tactile::gui {

class tileset_rubber_band final : public QRubberBand
{
  Q_OBJECT

 public:
  explicit tileset_rubber_band(QWidget* parent = nullptr);

  void mouse_pressed(const QPoint& pos);

  void mouse_moved(const QPoint& pos);

  void mouse_released();

  void set_tile_width(tile_width tileWidth) noexcept;

  void set_tile_height(tile_height tileHeight) noexcept;

 signals:
  void finished_selection(const core::tileset::selection& selection);

 private:
  tile_width m_tileWidth{};
  tile_height m_tileHeight{};
  QPoint m_origin;
  QPoint m_lastMousePos;
  row_t m_originRow;
  col_t m_originCol;

  void fit_geometry();

  [[nodiscard]] auto adjusted_geometry() const -> QRect;

  [[nodiscard]] auto get_selection() const -> core::tileset::selection;
};

}  // namespace tactile::gui

#pragma once
#include <QScrollArea>

namespace tactile {

class TilesetScrollArea final : public QScrollArea {
  Q_OBJECT

 public:
  explicit TilesetScrollArea(QWidget* parent = nullptr);

  ~TilesetScrollArea() noexcept override;

 signals:
  void s_clicked_tile_sheet_cell(int x, int y);

  void s_updated_tile_sheet_cell_selection(int x, int y);

 protected:
  void mousePressEvent(QMouseEvent* event) override;

  void mouseMoveEvent(QMouseEvent* event) override;
};

}  // namespace tactile

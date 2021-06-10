#pragma once

#include <QLabel>   // QLabel
#include <QPoint>   // QPoint
#include <QWidget>  // QWidget

#include "forward_declare.hpp"
#include "tileset.hpp"

TACTILE_FORWARD_DECLARE(tactile, TilesetRubberBand)

namespace tactile {

class TilesetImageWidget final : public QWidget
{
  Q_OBJECT

 public:
  explicit TilesetImageWidget(const core::Tileset& tileset,
                              QWidget* parent = nullptr);

  ~TilesetImageWidget() noexcept override;

 signals:
  void S_SetTilesetSelection(const core::tileset_selection& selection);

 protected:
  void mousePressEvent(QMouseEvent* event) override;

  void mouseReleaseEvent(QMouseEvent* event) override;

  void mouseMoveEvent(QMouseEvent* event) override;

 private:
  QLayout* mLayout{};
  QLabel* mImageLabel{};
  TilesetRubberBand* mRubberBand{};
};

}  // namespace tactile

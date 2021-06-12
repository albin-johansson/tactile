#pragma once

#include <QLabel>   // QLabel
#include <QPoint>   // QPoint
#include <QWidget>  // QWidget

#include "tileset.hpp"

namespace tactile {

class TilesetRubberBand;

class TilesetImageWidget final : public QWidget
{
  Q_OBJECT

 public:
  explicit TilesetImageWidget(const core::Tileset& tileset,
                              QWidget* parent = nullptr);

  ~TilesetImageWidget() noexcept override;

 signals:
  void S_SetTilesetSelection(const core::TilesetSelection& selection);

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

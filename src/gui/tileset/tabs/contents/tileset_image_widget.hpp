#pragma once

#include <QWidget>  // QWidget

#include "core_fwd.hpp"
#include "gui_fwd.hpp"

class QLabel;

namespace tactile {

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

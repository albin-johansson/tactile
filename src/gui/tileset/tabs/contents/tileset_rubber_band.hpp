#pragma once

#include <QRubberBand>  // QRubberBand

#include "tile_height.hpp"
#include "tile_width.hpp"
#include "tileset.hpp"

namespace tactile {

class TilesetRubberBand final : public QRubberBand
{
  Q_OBJECT

 public:
  explicit TilesetRubberBand(QWidget* parent = nullptr);

  void SetTileWidth(tile_width tileWidth) noexcept;

  void SetTileHeight(tile_height tileHeight) noexcept;

 signals:
  void S_FinishedSelection(const core::tileset_selection& selection);

 public slots:
  void MousePressed(const QPoint& pos);

  void MouseMoved(const QPoint& pos);

  void MouseReleased();

 private:
  tile_width mTileWidth;
  tile_height mTileHeight;
  QPoint mOrigin;
  QPoint mLastMousePos;
  row_t mOriginRow;
  col_t mOriginCol;

  void FitGeometry();

  [[nodiscard]] auto GetAdjustedGeometry() const -> QRect;

  [[nodiscard]] auto GetSelection() const -> core::tileset_selection;
};

}  // namespace tactile

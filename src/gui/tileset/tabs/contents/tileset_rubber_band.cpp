#include "tileset_rubber_band.hpp"

#include <algorithm>  // max

namespace tactile {

TilesetRubberBand::TilesetRubberBand(QWidget* parent)
    : QRubberBand{QRubberBand::Rectangle, parent}
{}

void TilesetRubberBand::SetTileWidth(const tile_width tileWidth) noexcept
{
  mTileWidth = tileWidth;
}

void TilesetRubberBand::SetTileHeight(const tile_height tileHeight) noexcept
{
  mTileHeight = tileHeight;
}

void TilesetRubberBand::MousePressed(const QPoint& pos)
{
  mOrigin = pos;

  mOriginRow = row_t{mOrigin.y() / mTileHeight.get()};
  mOriginCol = col_t{mOrigin.x() / mTileWidth.get()};

  mLastMousePos = mOrigin;

  setGeometry(mOrigin.x(), mOrigin.y(), mTileWidth.get(), mTileHeight.get());

  FitGeometry();
  show();
}

void TilesetRubberBand::MouseMoved(const QPoint& pos)
{
  mLastMousePos = pos;
  setGeometry(QRect{mOrigin, pos}.normalized());
  FitGeometry();
}

void TilesetRubberBand::MouseReleased()
{
  emit S_FinishedSelection(GetSelection());
}

void TilesetRubberBand::FitGeometry()
{
  setGeometry(GetAdjustedGeometry());
}

auto TilesetRubberBand::GetAdjustedGeometry() const -> QRect
{
  const auto& current = geometry();
  const auto tw = mTileWidth.get();
  const auto th = mTileHeight.get();

  QRect adjusted;
  adjusted.setX(current.x() - (current.x() % tw));
  adjusted.setY(current.y() - (current.y() % th));

  // Subtracts 1 at the end to avoid size that is 1 pixel larger than tile size
  adjusted.setRight(current.right() - (current.right() % tw) - 1);
  adjusted.setBottom(current.bottom() - (current.bottom() % th) - 1);

  adjusted.setWidth(std::max(tw, adjusted.width()));
  adjusted.setHeight(std::max(th, adjusted.height()));

  if (const row_t currentRow{mLastMousePos.y() / th}; currentRow != mOriginRow)
  {
    adjusted.setHeight(adjusted.height() + th);
  }

  if (const col_t currentCol{mLastMousePos.x() / tw}; currentCol != mOriginCol)
  {
    adjusted.setWidth(adjusted.width() + tw);
  }

  return adjusted;
}

auto TilesetRubberBand::GetSelection() const -> core::tileset_selection
{
  const auto& geo = geometry();
  const auto tw = mTileWidth.get();
  const auto th = mTileHeight.get();

  const core::Position tl{row_t{geo.y() / th}, col_t{geo.x() / tw}};
  const core::Position br{std::max(row_t{geo.bottom() / th}, tl.Row()),
                          std::max(col_t{geo.right() / tw}, tl.Column())};
  return {tl, br};
}

}  // namespace tactile

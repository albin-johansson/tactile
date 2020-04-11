
#include "tile_sheet.h"

#include <QImage>

#include "tactile_types.h"

namespace tactile {

TileSheet::TileSheet(const Shared<QImage>& image, int tileWidth, int tileHeight)
    : m_sheet{image},
      m_tileWidth{(tileWidth < 1) ? 1 : tileWidth},
      m_tileHeight{(tileHeight < 1) ? 1 : tileHeight},
      m_firstID{1}
{
  if (!image) {
    throw BadArg{"Cannot create tile sheet from null image!"};
  }

  m_rows = height() / m_tileHeight;
  m_cols = width() / m_tileWidth;
  m_nTiles = m_rows * m_cols;
}

TileSheet::TileSheet(const TileSheet& other) noexcept  // NOLINT
    : m_sheet{other.m_sheet},
      m_tileWidth{other.m_tileWidth},
      m_tileHeight{other.m_tileHeight},
      m_nTiles{other.m_nTiles},
      m_rows{other.m_rows},
      m_cols{other.m_cols},
      m_firstID{other.m_firstID},
      m_selection{other.m_selection}
{}

TileSheet::TileSheet(TileSheet&& other) noexcept
    : m_sheet{std::move(other.m_sheet)},
      m_tileWidth{other.m_tileWidth},
      m_tileHeight{other.m_tileHeight},
      m_nTiles{other.m_nTiles},
      m_rows{other.m_rows},
      m_cols{other.m_cols},
      m_firstID{other.m_firstID},
      m_selection{std::move(other.m_selection)}
{}

TileSheet::~TileSheet() noexcept = default;

Unique<TileSheet> TileSheet::unique(const Shared<QImage>& image,
                                    int tileWidth,
                                    int tileHeight)
{
  return std::make_unique<TileSheet>(image, tileWidth, tileHeight);
}

Shared<TileSheet> TileSheet::shared(const Shared<QImage>& image,
                                    int tileWidth,
                                    int tileHeight)
{
  return std::make_shared<TileSheet>(image, tileWidth, tileHeight);
}

void TileSheet::set_first_id(TileID firstID) noexcept
{
  if (firstID > 0) {
    m_firstID = firstID;
  }
}

void TileSheet::select(int x, int y) noexcept
{
  const auto tile = tile_at(x, y);
  if (tile != empty) {
    m_selection.emplace(tile);
  }
}

void TileSheet::clear_selection() noexcept
{
  m_selection.clear();
}

bool TileSheet::contains(TileID id) const noexcept
{
  return id >= first_id() && id <= last_id();
}

TileID TileSheet::tile_at(int x, int y) const noexcept
{
  if (x < 0 || y < 0 || x > width() || y > height()) {
    return empty;
  } else {
    const auto row = (y / m_tileHeight);
    const auto col = (x / m_tileWidth);
    const auto index = row * m_cols + col;
    return m_firstID + index;
  }
}

int TileSheet::width() const noexcept
{
  return m_sheet->width();
}

int TileSheet::height() const noexcept
{
  return m_sheet->height();
}

}  // namespace tactile

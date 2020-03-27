#include "tile_sheet.h"

#include <QImage>
#include <stdexcept>

namespace tactile {

TileSheet::TileSheet(const SharedPtr<QImage>& image, int size)
    : m_sheet{image}, m_firstID{1}
{
  if (!image) {
    throw std::invalid_argument{"Cannot create tile sheet from null image!"};
  }

  m_size = (size < 1) ? 1 : size;
  m_nTiles = (image->height() / m_size) * (image->width() / m_size);
  m_rows = height() / m_size;
  m_cols = width() / m_size;

  clear_selection();
}

TileSheet::~TileSheet() noexcept = default;

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
    const auto row = (y / m_size);
    const auto col = (x / m_size);
    const auto index = row * cols() + col;
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

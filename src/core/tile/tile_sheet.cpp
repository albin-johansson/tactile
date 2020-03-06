#include "tile_sheet.h"

#include <QImage>
#include <stdexcept>

namespace tactile {
namespace {

[[nodiscard]] int calc_tile_amount(int width, int height, int size) noexcept
{
  size = (size < 1) ? 1 : 0;
  return (height / size) * (width / size);
}

}

TileSheet::TileSheet(const SharedPtr<QImage>& image, int size)
    : m_sheet{image},
      m_nTiles{calc_tile_amount(image->width(), image->height(), size)},
      m_firstID{1}
{
  if (!image) {
    throw std::invalid_argument{"Cannot create tile sheet from null image!"};
  }
}

void TileSheet::set_first_id(TileID firstID) noexcept
{
  m_firstID = firstID;
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

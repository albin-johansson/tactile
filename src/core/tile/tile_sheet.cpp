#include "tile_sheet.h"

#include <QImage>
#include <stdexcept>

namespace tactile {
namespace {

[[nodiscard]] int calc_tile_amount(int width, int height, int size) noexcept
{
  size = (size < 1) ? 1 : size;
  return (height / size) * (width / size);
}

}  // namespace

TileSheet::TileSheet(const SharedPtr<QImage>& image, int size)
    : m_sheet{image}, m_firstID{1}
{
  if (!image) {
    throw std::invalid_argument{"Cannot create tile sheet from null image!"};
  }
  m_nTiles = calc_tile_amount(image->width(), image->height(), size);
}

void TileSheet::set_first_id(TileID firstID) noexcept
{
  if (firstID > 0) {
    m_firstID = firstID;
  }
}

bool TileSheet::contains(TileID id) const noexcept
{
  return id >= first_id() && id <= last_id();
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

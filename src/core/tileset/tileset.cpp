
#include "tileset.hpp"

#include <cassert>  // assert

#include "algorithm.hpp"
#include "tactile_error.hpp"
#include "types.hpp"

namespace tactile::core {

tileset::tileset(const QImage& image,
                 tile_width tileWidth,
                 tile_height tileHeight)
    : m_sheet{QPixmap::fromImage(image)},
      m_tileWidth{at_least(tileWidth, 1_tw)},
      m_tileHeight{at_least(tileHeight, 1_th)}
{
  if (m_sheet.isNull()) {
    throw tactile_error{"Cannot create tileset from null image!"};
  }

  m_rows = height() / m_tileHeight.get();
  m_cols = width() / m_tileWidth.get();
  m_nTiles = m_rows * m_cols;
}

tileset::tileset(const QString& path,
                 tile_width tileWidth,
                 tile_height tileHeight)
    : tileset{QImage{path}, tileWidth, tileHeight}
{}

void tileset::set_first_id(tile_id firstID) noexcept
{
  assert(firstID.get() > 0);
  m_firstID = firstID;
}

void tileset::set_selection(const position& topLeft,
                            const position& bottomRight)
{
  m_selection = {topLeft, bottomRight};
}

auto tileset::contains(tile_id id) const noexcept -> bool
{
  return (id >= first_id()) && (id <= last_id());
}

auto tileset::tile_at(row r, col c) const -> tile_id
{
  if ((r >= 0_row) && (c >= 0_col) && (r < row{rows()}) && (c < col{cols()})) {
    const auto index = r.get() * m_cols + c.get();
    return m_firstID + tile_id{index};
  } else {
    return empty;
  }
}

auto tileset::width() const -> int
{
  return m_sheet.width();
}

auto tileset::height() const -> int
{
  return m_sheet.height();
}

auto tileset::rows() const noexcept -> int
{
  return m_rows;
}

auto tileset::cols() const noexcept -> int
{
  return m_cols;
}

auto tileset::num_tiles() const noexcept -> int
{
  return m_nTiles;
}

auto tileset::first_id() const noexcept -> tile_id
{
  return m_firstID;
}

auto tileset::last_id() const noexcept -> tile_id
{
  return first_id() + tile_id{num_tiles()};
}

auto tileset::image() const -> const QPixmap&
{
  return m_sheet;
}

auto tileset::get_tile_width() const noexcept -> tile_width
{
  return m_tileWidth;
}

auto tileset::get_tile_height() const noexcept -> tile_height
{
  return m_tileHeight;
}

auto tileset::get_selection() const noexcept -> const selection&
{
  return m_selection;
}

}  // namespace tactile::core

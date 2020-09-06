
#include "tileset.hpp"

#include <cassert>  // assert
#include <utility>  // move, ssize

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

// void tileset::select(int x, int y)
//{
//  if (const auto tile = tile_at(x, y); tile != empty) {
//    m_selection.emplace(tile);
//  }
//}

void tileset::set_selection(position topLeft, position bottomRight)
{
//  clear_selection();

  qDebug("tileset > setting selection with TL: (%i, %i) and BR: (%i, %i)",
         topLeft.get_row(),
         topLeft.get_col(),
         bottomRight.get_row(),
         bottomRight.get_col());
  m_selection = {topLeft, bottomRight};
  //
  //  if (topLeft == bottomRight) {
  //    if (const auto tile = tile_at(topLeft.get_row(), topLeft.get_col());
  //        tile != empty) {
  //      m_selection.emplace(tile);
  //    }
  //    return;
  //  }
  //
  //  const auto endRow = bottomRight.get_row();
  //  const auto endCol = bottomRight.get_col();
  //
  //  for (row r = topLeft.get_row(); r < endRow; ++r) {
  //    for (col c = topLeft.get_col(); c < endCol; ++c) {
  //      if (const auto tile = tile_at(r, c); tile != empty) {
  //        m_selection.emplace(tile);
  //        qDebug("tileset > added (%i, %i) to selection", r.get(), c.get());
  //      }
  //    }
  //  }
}

// void tileset::clear_selection() noexcept
//{
//  m_selection.clear();
//}

auto tileset::contains(tile_id id) const noexcept -> bool
{
  return (id >= first_id()) && (id <= last_id());
}

auto tileset::tile_at(int x, int y) const -> tile_id
{
  if (x < 0 || y < 0 || x > width() || y > height()) {
    return empty;
  } else {
    const auto row = (y / m_tileHeight.get());
    const auto col = (x / m_tileWidth.get());
    const auto index = row * m_cols + col;
    return m_firstID + tile_id{index};
  }
}

auto tileset::tile_at(row r, col c) const -> tile_id
{
  if (r < 0_row || c < 0_col || r > row{rows()} || c > col{cols()}) {
    return empty;
  } else {
    const auto index = r.get() * m_cols + c.get();
    return m_firstID + tile_id{index};
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

// auto tileset::begin() const noexcept -> const_iterator
//{
//  return m_selection.begin();
//}
//
// auto tileset::end() const noexcept -> const_iterator
//{
//  return m_selection.end();
//}

// auto tileset::num_selected() const noexcept -> int
//{
//  return static_cast<int>(m_selection.size());
//}

}  // namespace tactile::core

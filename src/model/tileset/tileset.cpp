
#include "tileset.hpp"

#include <cassert>  // assert
#include <utility>  // move

#include "algorithm.hpp"
#include "tactile_error.hpp"
#include "tactile_types.hpp"

namespace tactile::model {

tileset::tileset(QImage image, int tileWidth, int tileHeight)
    : m_sheet{std::move(image)},
      m_tileWidth{at_least(tileWidth, 1)},
      m_tileHeight{at_least(tileHeight, 1)}
{
  if (m_sheet.isNull()) {
    throw tactile_error{"Cannot create tileset from null image!"};
  }

  m_rows = height() / m_tileHeight;
  m_cols = width() / m_tileWidth;
  m_nTiles = m_rows * m_cols;
}

tileset::tileset(const QString& path, int tileWidth, int tileHeight)
    : tileset{QImage{path}, tileWidth, tileHeight}
{}

void tileset::set_first_id(tile_id firstID) noexcept
{
  assert(firstID.get() > 0);
  m_firstID = firstID;
}

void tileset::select(int x, int y)
{
  if (const auto tile = tile_at(x, y); tile != empty) {
    m_selection.emplace(tile);
  }
}

void tileset::clear_selection() noexcept
{
  m_selection.clear();
}

auto tileset::contains(tile_id id) const noexcept -> bool
{
  return (id >= first_id()) && (id <= last_id());
}

auto tileset::tile_at(int x, int y) const noexcept -> tile_id
{
  if (x < 0 || y < 0 || x > width() || y > height()) {
    return empty;
  } else {
    const auto row = (y / m_tileHeight);
    const auto col = (x / m_tileWidth);
    const auto index = row * m_cols + col;
    return m_firstID + tile_id{index};
  }
}

auto tileset::width() const noexcept -> int
{
  return m_sheet.width();
}

auto tileset::height() const noexcept -> int
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

auto tileset::tiles() const noexcept -> int
{
  return m_nTiles;
}

auto tileset::first_id() const noexcept -> tile_id
{
  return m_firstID;
}

auto tileset::last_id() const noexcept -> tile_id
{
  return first_id() + tile_id{tiles()};
}

auto tileset::tile_width() const noexcept -> int
{
  return m_tileWidth;
}

auto tileset::tile_height() const noexcept -> int
{
  return m_tileHeight;
}

auto tileset::begin() const noexcept -> tileset::const_iterator
{
  return m_selection.begin();
}

auto tileset::end() const noexcept -> tileset::const_iterator
{
  return m_selection.end();
}

auto tileset::num_selected() const noexcept -> int
{
  return m_selection.size();
}

}  // namespace tactile::model

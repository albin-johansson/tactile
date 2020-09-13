
#include "tileset.hpp"

#include <cassert>  // assert

#include "algorithm.hpp"
#include "tactile_error.hpp"
#include "types.hpp"

namespace tactile::core {
namespace {

auto create_source_rect_cache(tile_id first,
                              tile_id last,
                              int nCols,
                              tile_width tw,
                              tile_height th)
    -> std::unordered_map<tile_id, QRect>
{
  std::unordered_map<tile_id, QRect> cache;

  const auto amount = (last + 1_t) - first;
  cache.reserve(amount.get());

  const auto tileWidth = tw.get();
  const auto tileHeight = th.get();

  for (tile_id id{first}; id <= last; ++id) {
    const auto index = id - first;

    const auto x = (index.get() % nCols) * tileWidth;
    const auto y = (index.get() / nCols) * tileHeight;

    cache.emplace(id, QRect{x, y, tileWidth, tileHeight});
  }

  return cache;
}

}  // namespace

tileset::tileset(tile_id firstID,
                 const QImage& image,
                 tile_width tileWidth,
                 tile_height tileHeight)
    : m_sheet{QPixmap::fromImage(image)},
      m_firstID{firstID},
      m_tileWidth{at_least(tileWidth, 1_tw)},
      m_tileHeight{at_least(tileHeight, 1_th)}
{
  if (m_sheet.isNull()) {
    throw tactile_error{"Cannot create tileset from null image!"};
  }

  m_rows = height() / m_tileHeight.get();
  m_cols = width() / m_tileWidth.get();
  m_nTiles = m_rows * m_cols;

  m_sourceRects = create_source_rect_cache(
      m_firstID, last_id(), m_cols, m_tileWidth, m_tileHeight);

  //  const auto tw = get_tile_width().get();
  //  const auto th = get_tile_height().get();
  //
  //  const auto first = first_id();
  //  const auto last = last_id();
  //
  //  for (tile_id id{first}; id <= last; ++id) {
  //    const auto index = id - first;
  //
  //    const auto row = index.get() / cols();
  //    const auto col = index.get() % cols();
  //
  //    const auto x = col * tw;
  //    const auto y = row * th;
  //
  //    m_sourceRects.emplace(id, QRect{x, y, tw, th});
  //  }
}

tileset::tileset(tile_id firstID,
                 const QString& path,
                 tile_width tileWidth,
                 tile_height tileHeight)
    : tileset{firstID, QImage{path}, tileWidth, tileHeight}
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

void tileset::clear_selection() noexcept
{
  m_selection.reset();
}

auto tileset::contains(tile_id id) const noexcept -> bool
{
  return (id >= first_id()) && (id <= last_id());
}

auto tileset::is_single_tile_selected() const noexcept -> bool
{
  return m_selection && (m_selection->topLeft == m_selection->bottomRight);
}

auto tileset::tile_at(const position& position) const -> tile_id
{
  const auto [row, col] = position.unpack();

  const auto endRow = row_t{rows()};
  const auto endCol = col_t{cols()};

  if ((row >= 0_row) && (col >= 0_col) && (row < endRow) && (col < endCol)) {
    const auto index = row.get() * m_cols + col.get();
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

auto tileset::image_source(tile_id id) const -> std::optional<QRect>
{
  if (const auto it = m_sourceRects.find(id); it != m_sourceRects.end()) {
    return it->second;
  } else {
    return std::nullopt;
  }
}

auto tileset::get_tile_width() const noexcept -> tile_width
{
  return m_tileWidth;
}

auto tileset::get_tile_height() const noexcept -> tile_height
{
  return m_tileHeight;
}

auto tileset::get_selection() const noexcept -> const std::optional<selection>&
{
  return m_selection;
}

}  // namespace tactile::core

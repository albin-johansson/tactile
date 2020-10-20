
#include "tileset.hpp"

#include "algorithm.hpp"
#include "czstring.hpp"
#include "tactile_error.hpp"

namespace tactile::core {
namespace {

auto create_source_rect_cache(tile_id first,
                              tile_id last,
                              col_t nCols,
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

    const auto x = (index.get() % nCols.get()) * tileWidth;
    const auto y = (index.get() / nCols.get()) * tileHeight;

    cache.emplace(id, QRect{x, y, tileWidth, tileHeight});
  }

  return cache;
}

}  // namespace

tileset::tileset(tile_id firstID,
                 const QImage& image,
                 tile_width tileWidth,
                 tile_height tileHeight)
    : m_image{QPixmap::fromImage(image)},
      m_firstID{firstID},
      m_tileWidth{tileWidth},
      m_tileHeight{tileHeight}
{
  if (m_image.isNull()) {
    throw tactile_error{"Cannot create tileset from null image!"};
  }

  if (m_tileWidth < 1_tw || m_tileHeight < 1_th) {
    throw tactile_error{"Invalid tileset tile dimensions!"};
  }

  m_numRows = row_t{height() / m_tileHeight.get()};
  m_numCols = col_t{width() / m_tileWidth.get()};
  m_numTiles = m_numRows.get() * m_numCols.get();
  m_lastID = m_firstID + tile_id{m_numTiles};
  m_sourceRects = create_source_rect_cache(
      m_firstID, m_lastID, m_numCols, m_tileWidth, m_tileHeight);
}

tileset::tileset(tile_id firstID,
                 const QString& path,
                 tile_width tileWidth,
                 tile_height tileHeight)
    : tileset{firstID, QImage{path}, tileWidth, tileHeight}
{}

void tileset::set_selection(const position& topLeft,
                            const position& bottomRight)
{
  m_selection = {topLeft, bottomRight};
}

void tileset::clear_selection() noexcept
{
  m_selection.reset();
}

void tileset::set_name(QString name)
{
  m_name = std::move(name);
}

void tileset::set_path(QString path)
{
  m_path = std::move(path);
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
  const auto endCol = col_t{col_count()};

  if ((row >= 0_row) && (col >= 0_col) && (row < endRow) && (col < endCol)) {
    const auto index = row.get() * m_numCols.get() + col.get();
    return m_firstID + tile_id{index};
  } else {
    return empty;
  }
}

auto tileset::width() const -> int
{
  return m_image.width();
}

auto tileset::height() const -> int
{
  return m_image.height();
}

auto tileset::image_source(tile_id id) const -> std::optional<QRect>
{
  if (const auto it = m_sourceRects.find(id); it != m_sourceRects.end()) {
    return it->second;
  } else {
    return std::nullopt;
  }
}

}  // namespace tactile::core

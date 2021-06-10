
#include "tileset.hpp"

#include <type_traits>  // is_final_v, is_move_constructible_v, ...

#include "algorithm.hpp"
#include "tactile_error.hpp"

namespace tactile::core {
namespace {

static_assert(std::is_final_v<Tileset>);

static_assert(std::is_move_constructible_v<Tileset>);
static_assert(std::is_move_assignable_v<Tileset>);

static_assert(std::is_copy_constructible_v<Tileset>);
static_assert(std::is_copy_assignable_v<Tileset>);

auto create_source_rect_cache(const tile_id first,
                              const tile_id last,
                              const col_t tileCount,
                              const tile_width tw,
                              const tile_height th) -> Tileset::rect_map
{
  Tileset::rect_map cache;

  const auto amount = (last + 1_t) - first;
  cache.reserve(amount.get());

  const auto tileWidth = tw.get();
  const auto tileHeight = th.get();

  for (tile_id id{first}; id <= last; ++id)
  {
    const auto index = id - first;

    const auto x = (index.get() % tileCount.get()) * tileWidth;
    const auto y = (index.get() / tileCount.get()) * tileHeight;

    cache.emplace(id, QRect{x, y, tileWidth, tileHeight});
  }

  return cache;
}

}  // namespace

Tileset::Tileset(const tile_id firstId,
                 const QImage& image,
                 const tile_width tileWidth,
                 const tile_height tileHeight)
    : m_image{QPixmap::fromImage(image)}
    , m_firstId{firstId}
    , m_tileWidth{tileWidth}
    , m_tileHeight{tileHeight}
{
  if (m_image.isNull())
  {
    throw TactileError{"Cannot create tileset from null image!"};
  }

  if (m_tileWidth < 1_tw || m_tileHeight < 1_th)
  {
    throw TactileError{"Invalid tileset tile dimensions!"};
  }

  m_nRows = row_t{height() / m_tileHeight.get()};
  m_nCols = col_t{width() / m_tileWidth.get()};
  m_tileCount = m_nRows.get() * m_nCols.get();
  m_lastId = m_firstId + tile_id{m_tileCount};
  m_sourceRects = create_source_rect_cache(m_firstId,
                                           m_lastId,
                                           m_nCols,
                                           m_tileWidth,
                                           m_tileHeight);
}

Tileset::Tileset(const tile_id firstID,
                 const QString& path,
                 const tile_width tileWidth,
                 const tile_height tileHeight)
    : Tileset{firstID, QImage{path}, tileWidth, tileHeight}
{
  m_path = QFileInfo{path};
}

void Tileset::set_selection(const tileset_selection& selection)
{
  m_selection = selection;
}

void Tileset::clear_selection() noexcept
{
  m_selection.reset();
}

void Tileset::set_name(QString name)
{
  m_name = std::move(name);
}

void Tileset::set_path(QFileInfo path)
{
  m_path = std::move(path);
}

auto Tileset::contains(const tile_id id) const noexcept -> bool
{
  return (id >= first_id()) && (id <= last_id());
}

auto Tileset::is_single_tile_selected() const noexcept -> bool
{
  return m_selection && (m_selection->topLeft == m_selection->bottomRight);
}

auto Tileset::tile_at(const Position& position) const -> tile_id
{
  const auto [row, col] = position.Unpack();

  const auto endRow = row_t{row_count()};
  const auto endCol = col_t{col_count()};

  if ((row >= 0_row) && (col >= 0_col) && (row < endRow) && (col < endCol))
  {
    const auto index = row.get() * m_nCols.get() + col.get();
    return m_firstId + tile_id{index};
  }
  else
  {
    return empty;
  }
}

auto Tileset::width() const -> int
{
  return m_image.width();
}

auto Tileset::height() const -> int
{
  return m_image.height();
}

auto Tileset::image_source(const tile_id id) const -> maybe<QRect>
{
  if (const auto it = m_sourceRects.find(id); it != m_sourceRects.end())
  {
    return it->second;
  }
  else
  {
    return std::nullopt;
  }
}

}  // namespace tactile::core

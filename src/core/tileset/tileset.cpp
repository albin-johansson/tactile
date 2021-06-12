
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
    : mImage{QPixmap::fromImage(image)}
    , mFirstId{firstId}
    , mTileWidth{tileWidth}
    , mTileHeight{tileHeight}
{
  if (mImage.isNull())
  {
    throw TactileError{"Cannot create tileset from null image!"};
  }

  if (mTileWidth < 1_tw || mTileHeight < 1_th)
  {
    throw TactileError{"Invalid tileset tile dimensions!"};
  }

  mRowCount = row_t{Height() / mTileHeight.get()};
  mColumnCount = col_t{Width() / mTileWidth.get()};
  mTileCount = mRowCount.get() * mColumnCount.get();
  mLastId = mFirstId + tile_id{mTileCount};
  mSourceRects = create_source_rect_cache(mFirstId,
                                          mLastId,
                                          mColumnCount,
                                          mTileWidth,
                                          mTileHeight);
}

Tileset::Tileset(const tile_id firstID,
                 const QString& path,
                 const tile_width tileWidth,
                 const tile_height tileHeight)
    : Tileset{firstID, QImage{path}, tileWidth, tileHeight}
{
  mPath = QFileInfo{path};
}

void Tileset::SetSelection(const TilesetSelection& selection)
{
  mSelection = selection;
}

void Tileset::ClearSelection() noexcept
{
  mSelection.reset();
}

void Tileset::SetName(QString name)
{
  mName = std::move(name);
}

void Tileset::SetPath(QFileInfo path)
{
  mPath = std::move(path);
}

auto Tileset::Contains(const tile_id id) const noexcept -> bool
{
  return (id >= FirstId()) && (id <= LastId());
}

auto Tileset::IsSingleTileSelected() const noexcept -> bool
{
  return mSelection && (mSelection->topLeft == mSelection->bottomRight);
}

auto Tileset::TileAt(const Position& position) const -> tile_id
{
  const auto [row, col] = position.Unpack();

  const auto endRow = row_t{RowCount()};
  const auto endCol = col_t{ColumnCount()};

  if ((row >= 0_row) && (col >= 0_col) && (row < endRow) && (col < endCol))
  {
    const auto index = row.get() * mColumnCount.get() + col.get();
    return mFirstId + tile_id{index};
  }
  else
  {
    return empty;
  }
}

auto Tileset::Width() const -> int
{
  return mImage.width();
}

auto Tileset::Height() const -> int
{
  return mImage.height();
}

auto Tileset::ImageSource(const tile_id id) const -> Maybe<QRect>
{
  if (const auto it = mSourceRects.find(id); it != mSourceRects.end())
  {
    return it->second;
  }
  else
  {
    return nothing;
  }
}

}  // namespace tactile::core

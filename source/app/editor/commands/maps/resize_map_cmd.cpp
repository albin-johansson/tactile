#include "resize_map_cmd.hpp"

#include "core/map.hpp"
#include "core/systems/map_system.hpp"

namespace tactile {

ResizeMapCmd::ResizeMapCmd(registry_ref registry, const usize nRows, const usize nCols)
    : command_base{"Resize Map"}
    , mRegistry{registry}
    , mRows{nRows}
    , mCols{nCols}
{}

void ResizeMapCmd::undo()
{
  auto& registry = mRegistry.get();
  sys::resize_map(registry, mPrevRows.value(), mPrevCols.value());

  if (IsLossyResize()) {
    mCache.RestoreTiles(registry);
  }
}

void ResizeMapCmd::redo()
{
  auto& registry = mRegistry.get();

  const auto& map = registry.ctx<MapInfo>();
  mPrevRows = map.row_count;
  mPrevCols = map.column_count;

  if (IsLossyResize()) {
    const auto rows = map.row_count;
    const auto cols = map.column_count;

    mCache.Clear();
    mCache.SaveTiles(registry,
                     tile_position::from(rows - (mPrevRows.value() - mRows), 0u),
                     tile_position::from(rows, cols));
    mCache.SaveTiles(registry,
                     tile_position::from(0u, cols - (mPrevCols.value() - mCols)),
                     tile_position::from(rows, cols));
  }

  sys::resize_map(registry, mRows, mCols);
}

auto ResizeMapCmd::IsLossyResize() const -> bool
{
  return mPrevRows > mRows || mPrevCols > mCols;
}

}  // namespace tactile

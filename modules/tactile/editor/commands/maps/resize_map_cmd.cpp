#include "resize_map_cmd.hpp"

#include "core/map.hpp"
#include "core/systems/map_system.hpp"

namespace tactile {

ResizeMapCmd::ResizeMapCmd(RegistryRef registry, const usize nRows, const usize nCols)
    : ACommand{"Resize Map"}
    , mRegistry{registry}
    , mRows{nRows}
    , mCols{nCols}
{}

void ResizeMapCmd::Undo()
{
  auto& registry = mRegistry.get();
  sys::ResizeMap(registry, mPrevRows.value(), mPrevCols.value());

  if (IsLossyResize()) {
    mCache.RestoreTiles(registry);
  }
}

void ResizeMapCmd::Redo()
{
  auto& registry = mRegistry.get();

  const auto& map = registry.ctx<Map>();
  mPrevRows = map.row_count;
  mPrevCols = map.column_count;

  if (IsLossyResize()) {
    const auto rows = map.row_count;
    const auto cols = map.column_count;

    mCache.Clear();
    mCache.SaveTiles(registry,
                     map_position::from(rows - (mPrevRows.value() - mRows), 0u),
                     map_position::from(rows, cols));
    mCache.SaveTiles(registry,
                     map_position::from(0u, cols - (mPrevCols.value() - mCols)),
                     map_position::from(rows, cols));
  }

  sys::ResizeMap(registry, mRows, mCols);
}

auto ResizeMapCmd::IsLossyResize() const -> bool
{
  return mPrevRows > mRows || mPrevCols > mCols;
}

}  // namespace tactile

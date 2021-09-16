#include "resize_map_cmd.hpp"

#include "core/ctx/map.hpp"
#include "core/systems/map_system.hpp"

namespace Tactile {

ResizeMapCmd::ResizeMapCmd(Ref<entt::registry> registry, const int nRows, const int nCols)
    : ACommand{"Resize Map"}
    , mRegistry{registry}
    , mRows{nRows}
    , mCols{nCols}
{}

void ResizeMapCmd::Undo()
{
  auto& registry = mRegistry.get();
  Sys::ResizeMap(registry, mPrevRows.value(), mPrevCols.value());

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
    mCache.SaveTiles(registry, {rows - (mPrevRows.value() - mRows), 0}, {rows, cols});
    mCache.SaveTiles(registry, {0, cols - (mPrevCols.value() - mCols)}, {rows, cols});
  }

  Sys::ResizeMap(registry, mRows, mCols);
}

auto ResizeMapCmd::IsLossyResize() const -> bool
{
  return mPrevRows > mRows || mPrevCols > mCols;
}

}  // namespace Tactile

#include "remove_column_cmd.hpp"

#include "core/algorithms/invoke_n.hpp"
#include "core/map.hpp"
#include "core/systems/map_system.hpp"

namespace tactile {

RemoveColumnCmd::RemoveColumnCmd(registry_ref registry)
    : command_base{"Remove Column(s)"}
    , mRegistry{registry}
{}

void RemoveColumnCmd::undo()
{
  invoke_n(mColumns, [this] { sys::add_column_to_map(mRegistry); });
  mCache.RestoreTiles(mRegistry);
}

void RemoveColumnCmd::redo()
{
  auto& registry = mRegistry.get();

  const auto& map = registry.ctx<MapInfo>();
  const auto begin = tile_position::from(0u, map.column_count - mColumns - 1u);
  const auto end = tile_position::from(map.row_count, map.column_count);

  mCache.Clear();
  mCache.SaveTiles(registry, begin, end);

  invoke_n(mColumns, [this] { sys::remove_column_from_map(mRegistry); });
}

auto RemoveColumnCmd::merge_with(const command_base& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const RemoveColumnCmd&>(cmd);

    mColumns += other.mColumns;
    mCache.MergeWith(other.mCache);

    return true;
  }

  return false;
}

}  // namespace tactile

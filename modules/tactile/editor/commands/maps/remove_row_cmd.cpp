#include "remove_row_cmd.hpp"

#include "core/algorithms/invoke_n.hpp"
#include "core/map.hpp"
#include "core/systems/map_system.hpp"

namespace tactile {

RemoveRowCmd::RemoveRowCmd(RegistryRef registry)
    : ACommand{"Remove Row(s)"}
    , mRegistry{registry}
{}

void RemoveRowCmd::Undo()
{
  invoke_n(mRows, [this] { sys::add_row_to_map(mRegistry); });
  mCache.RestoreTiles(mRegistry);
}

void RemoveRowCmd::Redo()
{
  auto& registry = mRegistry.get();

  const auto& map = registry.ctx<MapInfo>();
  const auto begin = tile_position::from(map.row_count - mRows - 1u, 0u);
  const auto end = tile_position::from(map.row_count, map.column_count);

  mCache.Clear();
  mCache.SaveTiles(mRegistry, begin, end);

  invoke_n(mRows, [this] { sys::remove_row_from_map(mRegistry); });
}

auto RemoveRowCmd::MergeWith(const ACommand& cmd) -> bool
{
  if (GetId() == cmd.GetId()) {
    const auto& other = dynamic_cast<const RemoveRowCmd&>(cmd);

    mRows += other.mRows;
    mCache.MergeWith(other.mCache);

    return true;
  }

  return false;
}

}  // namespace tactile

#include "remove_column_cmd.hpp"

#include "core/algorithms/invoke_n.hpp"
#include "core/map.hpp"
#include "core/systems/map_system.hpp"

namespace tactile {

RemoveColumnCmd::RemoveColumnCmd(RegistryRef registry)
    : ACommand{"Remove Column(s)"}
    , mRegistry{registry}
{}

void RemoveColumnCmd::Undo()
{
  invoke_n(mColumns, [this] { sys::AddColumn(mRegistry); });
  mCache.RestoreTiles(mRegistry);
}

void RemoveColumnCmd::Redo()
{
  auto& registry = mRegistry.get();

  const auto& map = registry.ctx<Map>();
  const auto begin = MapPosition::From(0u, map.column_count - mColumns - 1u);
  const auto end = MapPosition::From(map.row_count, map.column_count);

  mCache.Clear();
  mCache.SaveTiles(registry, begin, end);

  invoke_n(mColumns, [this] { sys::RemoveColumn(mRegistry); });
}

auto RemoveColumnCmd::MergeWith(const ACommand& cmd) -> bool
{
  if (GetId() == cmd.GetId()) {
    const auto& other = dynamic_cast<const RemoveColumnCmd&>(cmd);

    mColumns += other.mColumns;
    mCache.MergeWith(other.mCache);

    return true;
  }

  return false;
}

}  // namespace tactile

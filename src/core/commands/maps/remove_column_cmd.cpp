#include "remove_column_cmd.hpp"

#include "core/algorithms/invoke_n.hpp"
#include "core/ctx/map.hpp"
#include "core/systems/map_system.hpp"

namespace Tactile {

RemoveColumnCmd::RemoveColumnCmd(Ref<entt::registry> registry)
    : ACommand{"Remove Column(s)"}
    , mRegistry{registry}
{}

void RemoveColumnCmd::Undo()
{
  InvokeN(mColumns, [this] { Sys::AddColumn(mRegistry); });
  mCache.RestoreTiles(mRegistry);
}

void RemoveColumnCmd::Redo()
{
  auto& registry = mRegistry.get();

  const auto& map = registry.ctx<Map>();
  const MapPosition begin{0, map.column_count - static_cast<int32>(mColumns) - 1};
  const MapPosition end{map.row_count, map.column_count};

  mCache.Clear();
  mCache.SaveTiles(registry, begin, end);

  InvokeN(mColumns, [this] { Sys::RemoveColumn(mRegistry); });
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

}  // namespace Tactile

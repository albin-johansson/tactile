#include "remove_row_cmd.hpp"

#include "core/algorithms/invoke_n.hpp"
#include "core/ctx/map.hpp"
#include "core/systems/map_system.hpp"

namespace Tactile {

RemoveRowCmd::RemoveRowCmd(Ref<entt::registry> registry)
    : ACommand{"Remove Row(s)"}
    , mRegistry{registry}
{}

void RemoveRowCmd::Undo()
{
  InvokeN(mRows, [this] { Sys::AddRow(mRegistry); });
  mCache.RestoreTiles(mRegistry);
}

void RemoveRowCmd::Redo()
{
  auto& registry = mRegistry.get();

  const auto& map = registry.ctx<Map>();
  const MapPosition begin{map.row_count - AsRow(mRows) - 1_row, 0_col};
  const MapPosition end{map.row_count, map.column_count};

  mCache.Clear();
  mCache.SaveTiles(mRegistry, begin, end);

  InvokeN(mRows, [this] { Sys::RemoveRow(mRegistry); });
}

auto RemoveRowCmd::MergeWith(const ACommand& cmd) -> bool
{
  if (GetId() == cmd.GetId())
  {
    const auto& other = dynamic_cast<const RemoveRowCmd&>(cmd);

    mRows += other.mRows;
    mCache.MergeWith(other.mCache);

    return true;
  }

  return false;
}

}  // namespace Tactile

#include "remove_row_cmd.hpp"

#include "core/algorithms/invoke_n.hpp"
#include "core/map.hpp"
#include "core/systems/map_system.hpp"

namespace Tactile {

RemoveRowCmd::RemoveRowCmd(RegistryRef registry)
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
  const auto begin = MapPosition::From(map.row_count - mRows - 1u, 0u);
  const auto end = MapPosition::From(map.row_count, map.column_count);

  mCache.Clear();
  mCache.SaveTiles(mRegistry, begin, end);

  InvokeN(mRows, [this] { Sys::RemoveRow(mRegistry); });
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

}  // namespace Tactile

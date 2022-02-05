#include "remove_column_cmd.hpp"

#include "core/algorithms/invoke_n.hpp"
#include "core/map.hpp"
#include "core/systems/map_system.hpp"

namespace tactile {

remove_column_cmd::remove_column_cmd(registry_ref registry)
    : command_base{"Remove Column(s)"}
    , mRegistry{registry}
{}

void remove_column_cmd::undo()
{
  invoke_n(mColumns, [this] { sys::add_column_to_map(mRegistry); });
  mCache.restore_tiles(mRegistry);
}

void remove_column_cmd::redo()
{
  auto& registry = mRegistry.get();

  const auto& map = registry.ctx<MapInfo>();
  const auto begin = tile_position::from(0u, map.column_count - mColumns - 1u);
  const auto end = tile_position::from(map.row_count, map.column_count);

  mCache.clear();
  mCache.save_tiles(registry, begin, end);

  invoke_n(mColumns, [this] { sys::remove_column_from_map(mRegistry); });
}

auto remove_column_cmd::merge_with(const command_base& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const remove_column_cmd&>(cmd);

    mColumns += other.mColumns;
    mCache.merge_with(other.mCache);

    return true;
  }

  return false;
}

}  // namespace tactile

#include "add_column_cmd.hpp"

#include "core/algorithms/invoke_n.hpp"
#include "core/systems/map_system.hpp"

namespace tactile {

AddColumnCmd::AddColumnCmd(registry_ref registry)
    : command_base{"Add Column(s)"}
    , mRegistry{registry}
{}

void AddColumnCmd::undo()
{
  invoke_n(mColumns, [this] { sys::remove_column_from_map(mRegistry); });
}

void AddColumnCmd::redo()
{
  invoke_n(mColumns, [this] { sys::add_column_to_map(mRegistry); });
}

auto AddColumnCmd::merge_with(const command_base& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const AddColumnCmd&>(cmd);
    mColumns += other.mColumns;
    return true;
  }

  return false;
}

}  // namespace tactile

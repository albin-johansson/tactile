#include "add_column_cmd.hpp"

#include "core/algorithms/invoke_n.hpp"
#include "core/systems/map_system.hpp"

namespace tactile {

AddColumnCmd::AddColumnCmd(RegistryRef registry)
    : ACommand{"Add Column(s)"}
    , mRegistry{registry}
{}

void AddColumnCmd::Undo()
{
  invoke_n(mColumns, [this] { sys::RemoveColumn(mRegistry); });
}

void AddColumnCmd::Redo()
{
  invoke_n(mColumns, [this] { sys::AddColumn(mRegistry); });
}

auto AddColumnCmd::MergeWith(const ACommand& cmd) -> bool
{
  if (GetId() == cmd.GetId()) {
    const auto& other = dynamic_cast<const AddColumnCmd&>(cmd);
    mColumns += other.mColumns;
    return true;
  }

  return false;
}

}  // namespace tactile

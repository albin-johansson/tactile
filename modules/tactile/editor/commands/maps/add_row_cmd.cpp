#include "add_row_cmd.hpp"

#include "core/algorithms/invoke_n.hpp"
#include "core/systems/map_system.hpp"

namespace tactile {

AddRowCmd::AddRowCmd(RegistryRef registry) : ACommand{"Add Row(s)"}, mRegistry{registry}
{}

void AddRowCmd::Undo()
{
  invoke_n(mRows, [this] { sys::remove_row_from_map(mRegistry); });
}

void AddRowCmd::Redo()
{
  invoke_n(mRows, [this] { sys::add_row_to_map(mRegistry); });
}

auto AddRowCmd::MergeWith(const ACommand& cmd) -> bool
{
  if (GetId() == cmd.GetId()) {
    const auto& other = dynamic_cast<const AddRowCmd&>(cmd);
    mRows += other.mRows;
    return true;
  }

  return false;
}

}  // namespace tactile

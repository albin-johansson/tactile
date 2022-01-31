#include "add_row_cmd.hpp"

#include "core/algorithms/invoke_n.hpp"
#include "core/systems/map_system.hpp"

namespace tactile {

AddRowCmd::AddRowCmd(registry_ref registry) : command_base{"Add Row(s)"}, mRegistry{registry}
{}

void AddRowCmd::undo()
{
  invoke_n(mRows, [this] { sys::remove_row_from_map(mRegistry); });
}

void AddRowCmd::redo()
{
  invoke_n(mRows, [this] { sys::add_row_to_map(mRegistry); });
}

auto AddRowCmd::merge_with(const command_base& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const AddRowCmd&>(cmd);
    mRows += other.mRows;
    return true;
  }

  return false;
}

}  // namespace tactile

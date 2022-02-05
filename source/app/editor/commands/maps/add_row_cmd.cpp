#include "add_row_cmd.hpp"

#include "core/algorithms/invoke_n.hpp"
#include "core/systems/map_system.hpp"

namespace tactile {

add_row_cmd::add_row_cmd(registry_ref registry)
    : command_base{"Add Row(s)"}
    , mRegistry{registry}
{}

void add_row_cmd::undo()
{
  invoke_n(mRows, [this] { sys::remove_row_from_map(mRegistry); });
}

void add_row_cmd::redo()
{
  invoke_n(mRows, [this] { sys::add_row_to_map(mRegistry); });
}

auto add_row_cmd::merge_with(const command_base& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const add_row_cmd&>(cmd);
    mRows += other.mRows;
    return true;
  }

  return false;
}

}  // namespace tactile

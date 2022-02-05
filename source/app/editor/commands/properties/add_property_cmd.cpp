#include "add_property_cmd.hpp"

#include <utility>  // move

#include "core/systems/context_system.hpp"
#include "core/systems/property_system.hpp"

namespace tactile {

add_property_cmd::add_property_cmd(registry_ref registry,
                                   std::string name,
                                   const attribute_type type)
    : command_base{"Add Property"}
    , mRegistry{registry}
    , mContextId{sys::current_context_id(mRegistry)}
    , mName{std::move(name)}
    , mType{type}
{}

void add_property_cmd::undo()
{
  auto& context = sys::get_context(mRegistry, mContextId);
  sys::remove_property(mRegistry, context, mName);
}

void add_property_cmd::redo()
{
  auto& context = sys::get_context(mRegistry, mContextId);
  sys::add_property(mRegistry, context, mName, mType);
}

}  // namespace tactile

#include "update_component_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace tactile {

update_component_cmd::update_component_cmd(registry_ref registry,
                                           const context_id contextId,
                                           const component_id componentId,
                                           std::string attribute,
                                           attribute_value value)
    : command_base{"update Component Value"}
    , mRegistry{registry}
    , mContextId{contextId}
    , mComponentId{componentId}
    , mAttributeName{std::move(attribute)}
    , mUpdatedValue{std::move(value)}
{}

void update_component_cmd::undo()
{
  auto& registry = mRegistry.get();

  sys::update_component(registry,
                        mContextId,
                        mComponentId,
                        mAttributeName,
                        mPreviousValue.value());

  mPreviousValue.reset();
}

void update_component_cmd::redo()
{
  auto& registry = mRegistry.get();

  mPreviousValue =
      sys::get_component_attribute(registry, mContextId, mComponentId, mAttributeName);
  sys::update_component(registry,
                        mContextId,
                        mComponentId,
                        mAttributeName,
                        mUpdatedValue);
}

auto update_component_cmd::merge_with(const command_base& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const update_component_cmd&>(cmd);

    const bool targetsSameAttribute = mContextId == other.mContextId &&
                                      mComponentId == other.mComponentId &&
                                      mAttributeName == other.mAttributeName;
    if (targetsSameAttribute) {
      mUpdatedValue = other.mUpdatedValue;
      return true;
    }
  }

  return false;
}

}  // namespace tactile
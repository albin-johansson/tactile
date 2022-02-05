#pragma once

#include <string>  // string

#include "core/components/component.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile.hpp"

namespace tactile {

class update_component_attr_cmd final : public command_base
{
 public:
  update_component_attr_cmd(registry_ref registry,
                            component_id id,
                            std::string attribute,
                            attribute_value value);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto merge_with(const command_base& cmd) -> bool override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::update_component_attribute;
  }

 private:
  registry_ref mRegistry;
  component_id mComponentId{};
  std::string mAttributeName;
  attribute_value mUpdatedValue;
  maybe<attribute_value> mPreviousValue;
};

}  // namespace tactile
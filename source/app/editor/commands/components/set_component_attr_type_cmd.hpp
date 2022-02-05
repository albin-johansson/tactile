#pragma once

#include <string>  // string

#include "core/components/component.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace tactile {

class set_component_attr_type_cmd final : public command_base {
 public:
  set_component_attr_type_cmd(registry_ref registry,
                              component_id id,
                              std::string attribute,
                              attribute_type type);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::set_component_attribute_type;
  }

 private:
  registry_ref mRegistry;
  component_id mComponentId;
  std::string mAttributeName;
  attribute_type mNewType;
  maybe<attribute_value> mPreviousValue;
};

}  // namespace tactile

#pragma once

#include <string>  // string

#include "core/attribute_value.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile.hpp"

namespace tactile {

class change_property_type_cmd final : public command_base {
 public:
  change_property_type_cmd(registry_ref registry, std::string name, attribute_type type);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::change_property_type;
  }

 private:
  registry_ref mRegistry;
  context_id mContextId;
  std::string mName;
  attribute_type mPropertyType;
  maybe<attribute_value> mPreviousValue;
};

}  // namespace tactile

#pragma once

#include <string>  // string

#include "core/attribute_value.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile.hpp"

namespace tactile {

class update_property_cmd final : public command_base
{
 public:
  update_property_cmd(registry_ref registry, std::string name, attribute_value value);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto merge_with(const command_base& cmd) -> bool override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::update_property;
  }

 private:
  registry_ref mRegistry;
  context_id mContextId;
  std::string mName;
  attribute_value mNewValue;
  maybe<attribute_value> mOldValue;
};

}  // namespace tactile

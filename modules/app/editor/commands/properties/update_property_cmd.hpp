#pragma once

#include <string>  // string

#include <entt/entt.hpp>  // registry

#include "core/attribute_value.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class UpdatePropertyCmd final : public command_base {
 public:
  UpdatePropertyCmd(registry_ref registry, std::string name, attribute_value value);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto merge_with(const command_base& cmd) -> bool override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return CommandId::UpdateProperty;
  }

 private:
  registry_ref mRegistry;
  context_id mContextId;
  std::string mName;
  attribute_value mNewValue;
  maybe<attribute_value> mOldValue;
};

}  // namespace tactile

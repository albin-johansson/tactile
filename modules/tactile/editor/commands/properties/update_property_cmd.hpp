#pragma once

#include <string>  // string

#include <entt/entt.hpp>  // registry
#include <tactile_def.hpp>

#include "core/attribute_value.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace tactile {

class UpdatePropertyCmd final : public ACommand {
 public:
  UpdatePropertyCmd(RegistryRef registry, std::string name, attribute_value value);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto MergeWith(const ACommand& cmd) -> bool override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::UpdateProperty;
  }

 private:
  RegistryRef mRegistry;
  ContextID mContextId;
  std::string mName;
  attribute_value mNewValue;
  Maybe<attribute_value> mOldValue;
};

}  // namespace tactile

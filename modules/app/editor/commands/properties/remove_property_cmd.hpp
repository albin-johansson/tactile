#pragma once

#include <string>  // string

#include <entt/entt.hpp>  // registry

#include "core/attribute_value.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class RemovePropertyCmd final : public ACommand {
 public:
  RemovePropertyCmd(RegistryRef registry, std::string name);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RemoveProperty;
  }

 private:
  RegistryRef mRegistry;
  context_id mContextId;
  std::string mName;
  maybe<attribute_value> mPreviousValue;
};

}  // namespace tactile

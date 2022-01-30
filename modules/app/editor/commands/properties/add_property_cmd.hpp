#pragma once

#include <string>  // string

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class AddPropertyCmd final : public ACommand {
 public:
  AddPropertyCmd(RegistryRef registry, std::string name, attribute_type type);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::AddProperty;
  }

 private:
  RegistryRef mRegistry;
  ContextID mContextId;
  std::string mName;
  attribute_type mType;
};

}  // namespace tactile

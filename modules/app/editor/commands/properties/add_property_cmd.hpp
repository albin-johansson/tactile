#pragma once

#include <string>  // string

#include <entt/entt.hpp>  // registry

#include "core/attribute_value.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class AddPropertyCmd final : public command_base {
 public:
  AddPropertyCmd(registry_ref registry, std::string name, attribute_type type);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return CommandId::AddProperty;
  }

 private:
  registry_ref mRegistry;
  context_id mContextId;
  std::string mName;
  attribute_type mType;
};

}  // namespace tactile

#pragma once

#include <string>  // string

#include <entt/entt.hpp>  // registry

#include "core/attribute_value.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class ChangePropertyTypeCmd final : public ACommand {
 public:
  ChangePropertyTypeCmd(RegistryRef registry, std::string name, attribute_type type);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::ChangePropertyType;
  }

 private:
  RegistryRef mRegistry;
  context_id mContextId;
  std::string mName;
  attribute_type mPropertyType;
  maybe<attribute_value> mPreviousValue;
};

}  // namespace tactile

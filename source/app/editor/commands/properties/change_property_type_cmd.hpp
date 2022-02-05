#pragma once

#include <string>  // string

#include <entt/entt.hpp>  // registry

#include "core/attribute_value.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile.hpp"

namespace tactile {

class ChangePropertyTypeCmd final : public command_base {
 public:
  ChangePropertyTypeCmd(registry_ref registry, std::string name, attribute_type type);

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

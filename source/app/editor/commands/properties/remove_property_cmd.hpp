#pragma once

#include <string>  // string

#include <entt/entt.hpp>  // registry

#include "core/attribute_value.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class RemovePropertyCmd final : public command_base {
 public:
  RemovePropertyCmd(registry_ref registry, std::string name);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::remove_property;
  }

 private:
  registry_ref mRegistry;
  context_id mContextId;
  std::string mName;
  maybe<attribute_value> mPreviousValue;
};

}  // namespace tactile
#pragma once

#include <string>  // string

#include "core/attribute.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile.hpp"

namespace tactile {

class remove_property_cmd final : public command_base
{
 public:
  remove_property_cmd(registry_ref registry, std::string name);

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

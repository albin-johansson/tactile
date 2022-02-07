#pragma once

#include <string>  // string

#include "core/attribute.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile.hpp"

namespace tactile {

class add_property_cmd final : public command_base
{
 public:
  add_property_cmd(registry_ref registry, std::string name, attribute_type type);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::add_property;
  }

 private:
  registry_ref mRegistry;
  context_id mContextId;
  std::string mName;
  attribute_type mType;
};

}  // namespace tactile

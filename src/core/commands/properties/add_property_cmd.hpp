#pragma once

#include <entt.hpp>  // registry
#include <string>    // string

#include "aliases/context_id.hpp"
#include "aliases/ref.hpp"
#include "core/commands/command.hpp"
#include "core/commands/command_id.hpp"
#include "core/properties/property_type.hpp"

namespace Tactile {

class AddPropertyCmd final : public ACommand
{
 public:
  AddPropertyCmd(Ref<entt::registry> registry,
                 std::string name,
                 PropertyType type);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::AddProperty;
  }

 private:
  Ref<entt::registry> mRegistry;
  ContextID mContextId;
  std::string mName;
  PropertyType mType;
};

}  // namespace Tactile

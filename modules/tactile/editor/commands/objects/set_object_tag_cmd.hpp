#pragma once

#include <string>  // string

#include <tactile-base/tactile_std.hpp>

#include <entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace Tactile {

struct Object;

class SetObjectTagCmd final : public ACommand
{
 public:
  SetObjectTagCmd(Ref<entt::registry> registry, ObjectID id, std::string tag);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto MergeWith(const ACommand& cmd) -> bool override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::SetObjectTag;
  }

 private:
  Ref<entt::registry> mRegistry;
  ObjectID mObjectId;
  std::string mNewTag;
  Maybe<std::string> mOldTag;

  [[nodiscard]] auto GetObject() -> Object&;
};

}  // namespace Tactile

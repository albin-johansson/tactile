#pragma once

#include <string>  // string

#include <tactile-base/tactile_std.hpp>

#include <entt.hpp>  // registry

#include "editor/commands/command.hpp"

namespace Tactile {

struct Object;

class AObjectCmd : public ACommand
{
 public:
  AObjectCmd(std::string name, Ref<entt::registry> registry, ObjectID id);

 protected:
  [[nodiscard]] auto GetTargetObject() -> Object&;

  [[nodiscard]] auto GetTargetObjectId() const noexcept -> ObjectID
  {
    return mObjectId;
  }

 private:
  Ref<entt::registry> mRegistry;
  ObjectID mObjectId;
};

}  // namespace Tactile

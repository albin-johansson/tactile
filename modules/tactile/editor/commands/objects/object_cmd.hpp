#pragma once

#include <string>  // string

#include <tactile_def.hpp>

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"

namespace Tactile {

struct Object;
struct PropertyContext;

class AObjectCmd : public ACommand {
 public:
  AObjectCmd(std::string name, Ref<entt::registry> registry, ObjectID id);

 protected:
  [[nodiscard]] auto GetTargetObject() -> Object&;

  [[nodiscard]] auto GetTargetObjectContext() const -> PropertyContext&;

  [[nodiscard]] auto GetTargetObjectId() const noexcept -> ObjectID { return mObjectId; }

 private:
  Ref<entt::registry> mRegistry;
  ObjectID mObjectId;
};

}  // namespace Tactile

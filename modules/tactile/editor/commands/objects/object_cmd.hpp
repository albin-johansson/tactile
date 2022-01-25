#pragma once

#include <string>  // string

#include <entt/entt.hpp>  // registry
#include <tactile_def.hpp>

#include "editor/commands/command.hpp"

namespace tactile {

struct Object;
struct attribute_context;

class AObjectCmd : public ACommand {
 public:
  AObjectCmd(std::string name, RegistryRef registry, ObjectID id);

 protected:
  [[nodiscard]] auto GetTargetObject() -> Object&;

  [[nodiscard]] auto GetTargetObjectContext() const -> attribute_context&;

  [[nodiscard]] auto GetTargetObjectId() const noexcept -> ObjectID { return mObjectId; }

 private:
  RegistryRef mRegistry;
  ObjectID mObjectId;
};

}  // namespace tactile

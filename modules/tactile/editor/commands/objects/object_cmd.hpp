#pragma once

#include <string>  // string

#include <entt/entt.hpp>
#include <tactile_def.hpp>

#include "core/components/attribute_context.hpp"
#include "core/components/object.hpp"
#include "editor/commands/command.hpp"

namespace tactile {

class AObjectCmd : public ACommand {
 public:
  AObjectCmd(std::string name, RegistryRef registry, ObjectID id);

 protected:
  [[nodiscard]] auto GetTargetObject() -> comp::object&;

  [[nodiscard]] auto GetTargetObjectContext() const -> comp::attribute_context&;

  [[nodiscard]] auto GetTargetObjectId() const noexcept -> ObjectID { return mObjectId; }

 private:
  RegistryRef mRegistry;
  ObjectID mObjectId;
};

}  // namespace tactile

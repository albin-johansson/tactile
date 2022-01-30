#pragma once

#include <string>  // string

#include <entt/entt.hpp>

#include "core/components/attribute_context.hpp"
#include "core/components/object.hpp"
#include "editor/commands/command.hpp"
#include "tactile_def.hpp"

namespace tactile {

class AObjectCmd : public ACommand {
 public:
  AObjectCmd(std::string name, RegistryRef registry, object_id id);

 protected:
  [[nodiscard]] auto GetTargetObject() -> comp::object&;

  [[nodiscard]] auto GetTargetObjectContext() const -> comp::attribute_context&;

  [[nodiscard]] auto GetTargetObjectId() const noexcept -> object_id { return mObjectId; }

 private:
  RegistryRef mRegistry;
  object_id mObjectId;
};

}  // namespace tactile

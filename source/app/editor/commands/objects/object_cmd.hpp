#pragma once

#include <string>  // string

#include "core/components/attribute_context.hpp"
#include "core/components/object.hpp"
#include "editor/commands/command.hpp"
#include "tactile.hpp"

namespace tactile {

class object_cmd : public command_base {
 public:
  object_cmd(std::string name, registry_ref registry, object_id id);

 protected:
  [[nodiscard]] auto target_object() -> comp::object&;

  [[nodiscard]] auto target_object_context() const -> comp::attribute_context&;

  [[nodiscard]] auto target_object_id() const noexcept -> object_id { return mObjectId; }

 private:
  registry_ref mRegistry;
  object_id mObjectId;
};

}  // namespace tactile

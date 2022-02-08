#pragma once

#include <string>  // string

#include "component_name_dialog.hpp"
#include "core/components/component.hpp"
#include "tactile.hpp"

namespace tactile {

class RenameComponentDialog final : public AComponentNameDialog
{
 public:
  RenameComponentDialog();

  void Open(std::string previousName, component_id id);

 protected:
  void on_accept(entt::dispatcher& dispatcher) override;

 private:
  maybe<component_id> mComponentId;
};

}  // namespace tactile

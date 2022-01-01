#pragma once

#include <string>  // string

#include <tactile_def.hpp>

#include "component_name_dialog.hpp"
#include "core/components/component.hpp"

namespace Tactile {

class RenameComponentDialog final : public AComponentNameDialog {
 public:
  RenameComponentDialog();

  void Open(std::string previousName, ComponentID id);

 protected:
  void OnAccept(entt::dispatcher& dispatcher) override;

 private:
  Maybe<ComponentID> mComponentId;
};

}  // namespace Tactile

#pragma once

#include <string>  // string

#include "component_name_dialog.hpp"
#include "core/components/component.hpp"
#include "tactile.hpp"

namespace tactile {

/**
 * \brief Used to change the name of a component definition.
 *
 * \ingroup gui
 */
class rename_component_dialog final : public component_name_dialog
{
 public:
  rename_component_dialog();

  void show(std::string previousName, component_id id);

 protected:
  void on_accept(entt::dispatcher& dispatcher) override;

 private:
  maybe<component_id> mComponentId;
};

}  // namespace tactile

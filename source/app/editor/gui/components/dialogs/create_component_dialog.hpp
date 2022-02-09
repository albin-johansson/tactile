#pragma once

#include "component_name_dialog.hpp"

namespace tactile {

/**
 * \brief Used to create new component definitions.
 *
 * \ingroup gui
 */
class create_component_dialog final : public component_name_dialog
{
 public:
  create_component_dialog();

  void show();

 protected:
  void on_accept(entt::dispatcher& dispatcher) override;
};

}  // namespace tactile
#pragma once

#include "dialog.hpp"

namespace tactile {

/**
 * \brief Provides information about the application.
 *
 * \ingroup gui
 */
class about_dialog final : public dialog_base
{
 public:
  about_dialog();

  void show();

 protected:
  void on_update(const Model& model, entt::dispatcher& dispatcher) override;
};

}  // namespace tactile

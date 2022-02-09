#pragma once

#include "dialog.hpp"

namespace tactile {

/**
 * \brief Provides information about third-party dependencies.
 */
class credits_dialog final : public dialog_base
{
 public:
  credits_dialog();

  void show();

 protected:
  void on_update(const document_model& model, entt::dispatcher& dispatcher) override;
};

}  // namespace tactile

#pragma once

#include <entt/entt.hpp>

#include "dialog.hpp"
#include "tactile.hpp"

namespace tactile {

class ResizeMapDialog final : public dialog_base
{
 public:
  TACTILE_DEFAULT_COPY(ResizeMapDialog)
  TACTILE_DEFAULT_MOVE(ResizeMapDialog)

  ResizeMapDialog();

  ~ResizeMapDialog() override = default;

  void Show(usize nCurrentRows, usize nCurrentColumns);

 protected:
  void on_update(const Model& model, entt::dispatcher& dispatcher) override;

  void on_accept(entt::dispatcher& dispatcher) override;

 private:
  usize mRows{};
  usize mColumns{};
};

}  // namespace tactile

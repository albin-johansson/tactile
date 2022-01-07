#pragma once

#include <entt/entt.hpp>
#include <tactile_def.hpp>

#include "dialog.hpp"

namespace Tactile {

class ResizeMapDialog final : public ADialog {
 public:
  ResizeMapDialog();

  ~ResizeMapDialog() override = default;

  void Show(usize nCurrentRows, usize nCurrentColumns);

 protected:
  void UpdateContents(const Model& model, entt::dispatcher& dispatcher) override;

  void OnAccept(entt::dispatcher& dispatcher) override;

 private:
  usize mRows{};
  usize mColumns{};
};

}  // namespace Tactile

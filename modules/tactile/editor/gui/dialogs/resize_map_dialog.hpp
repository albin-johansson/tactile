#pragma once

#include <tactile_def.hpp>

#include <entt/entt.hpp>

#include "dialog.hpp"

namespace Tactile {

class ResizeMapDialog final : public ADialog {
 public:
  ResizeMapDialog();

  ~ResizeMapDialog() override = default;

  void Show(usize nCurrentRows, usize nCurrentColumns);

 protected:
  void UpdateContents(const entt::registry& registry,
                      entt::dispatcher& dispatcher) override;

  void OnAccept(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsCurrentInputValid(const entt::registry& registry) const
      -> bool override;

 private:
  usize mRows{};
  usize mColumns{};
};

}  // namespace Tactile

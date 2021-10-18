#pragma once

#include "tactile/editor/shortcuts/shortcut.hpp"

namespace Tactile {

class EnableStampShortcut final : public AShortcut
{
 public:
  EnableStampShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

}  // namespace Tactile

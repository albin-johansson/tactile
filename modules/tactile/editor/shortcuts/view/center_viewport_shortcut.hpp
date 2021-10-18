#pragma once

#include "tactile/editor/shortcuts/shortcut.hpp"

namespace Tactile {

class CenterViewportShortcut final : public AShortcut
{
 public:
  CenterViewportShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

}  // namespace Tactile

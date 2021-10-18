#pragma once

#include "tactile/editor/shortcuts/shortcut.hpp"

namespace Tactile {

class RemoveRowShortcut final : public AShortcut
{
 public:
  RemoveRowShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

}  // namespace Tactile

#pragma once

#include "editor/shortcuts/shortcut.hpp"

namespace Tactile {

class RemoveColumnShortcut final : public AShortcut
{
 public:
  RemoveColumnShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

}  // namespace Tactile

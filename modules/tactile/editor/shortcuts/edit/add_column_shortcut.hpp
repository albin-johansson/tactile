#pragma once

#include "tactile/editor/shortcuts/shortcut.hpp"

namespace Tactile {

class AddColumnShortcut final : public AShortcut
{
 public:
  AddColumnShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

}  // namespace Tactile

#pragma once

#include "tactile/editor/shortcuts/shortcut.hpp"

namespace Tactile {

class AddTilesetShortcut final : public AShortcut
{
 public:
  AddTilesetShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

}  // namespace Tactile

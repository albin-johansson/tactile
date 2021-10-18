#pragma once

#include "tactile/editor/shortcuts/shortcut.hpp"

namespace Tactile {

class ToggleGridShortcut final : public AShortcut
{
 public:
  ToggleGridShortcut();

  void Activate(entt::dispatcher& dispatcher) override;
};

}  // namespace Tactile

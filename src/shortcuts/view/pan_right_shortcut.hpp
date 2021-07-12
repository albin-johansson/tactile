#pragma once

#include "shortcuts/shortcut.hpp"

namespace Tactile {

class PanRightShortcut final : public AShortcut
{
 public:
  PanRightShortcut();

  void Activate(entt::dispatcher& dispatcher) override;
};

}  // namespace Tactile

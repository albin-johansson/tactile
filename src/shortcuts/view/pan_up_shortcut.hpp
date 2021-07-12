#pragma once

#include "shortcuts/shortcut.hpp"

namespace Tactile {

class PanUpShortcut final : public AShortcut
{
 public:
  PanUpShortcut();

  void Activate(entt::dispatcher& dispatcher) override;
};

}  // namespace Tactile

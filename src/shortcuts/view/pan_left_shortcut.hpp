#pragma once

#include "shortcuts/shortcut.hpp"

namespace Tactile {

class PanLeftShortcut final : public AShortcut
{
 public:
  PanLeftShortcut();

  void Activate(entt::dispatcher& dispatcher) override;
};

}  // namespace Tactile

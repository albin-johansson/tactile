#pragma once

#include "shortcuts/shortcut.hpp"

namespace Tactile {

class PanDownShortcut final : public AShortcut
{
 public:
  PanDownShortcut();

  void Activate(entt::dispatcher& dispatcher) override;
};

}  // namespace Tactile

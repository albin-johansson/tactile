#pragma once

#include "shortcuts/shortcut.hpp"

namespace Tactile {

class SaveShortcut final : public AShortcut
{
 public:
  SaveShortcut();

  void Activate(entt::dispatcher& dispatcher) override;
};

}  // namespace Tactile

#pragma once

#include "shortcuts/shortcut.hpp"

namespace Tactile {

class SaveAsShortcut final : public AShortcut
{
 public:
  SaveAsShortcut();

  void Activate(entt::dispatcher& dispatcher) override;
};

}  // namespace Tactile

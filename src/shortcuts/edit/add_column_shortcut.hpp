#pragma once

#include "shortcuts/shortcut.hpp"

namespace Tactile {

class AddColumnShortcut final : public AShortcut
{
 public:
  AddColumnShortcut();

  void Activate(entt::dispatcher& dispatcher) override;
};

}  // namespace Tactile

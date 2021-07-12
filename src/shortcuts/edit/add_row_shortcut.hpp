#pragma once

#include "shortcuts/shortcut.hpp"

namespace Tactile {

class AddRowShortcut final : public AShortcut
{
 public:
  AddRowShortcut();

  void Activate(entt::dispatcher& dispatcher) override;
};

}  // namespace Tactile

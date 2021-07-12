#pragma once

#include "shortcuts/shortcut.hpp"

namespace Tactile {

class EnableStampShortcut final : public AShortcut
{
 public:
  EnableStampShortcut();

  void Activate(entt::dispatcher& dispatcher) override;
};

}  // namespace Tactile

#pragma once

#include "tactile/editor/shortcuts/shortcut.hpp"

namespace Tactile {

class OpenSettingsShortcut final : public AShortcut
{
 public:
  OpenSettingsShortcut();

  void Activate(entt::dispatcher& dispatcher) override;
};

}  // namespace Tactile

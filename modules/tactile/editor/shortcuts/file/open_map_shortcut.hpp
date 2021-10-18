#pragma once

#include "tactile/editor/shortcuts/shortcut.hpp"

namespace Tactile {

class OpenMapShortcut final : public AShortcut
{
 public:
  OpenMapShortcut();

  void Activate(entt::dispatcher& dispatcher) override;
};

}  // namespace Tactile

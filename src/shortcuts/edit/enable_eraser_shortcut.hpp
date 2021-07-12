#pragma once

#include "shortcuts/shortcut.hpp"

namespace Tactile {

class EnableEraserShortcut final : public AShortcut
{
 public:
  EnableEraserShortcut();

  void Activate(entt::dispatcher& dispatcher) override;
};

}  // namespace Tactile

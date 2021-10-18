#pragma once

#include "tactile/editor/shortcuts/shortcut.hpp"

namespace Tactile {

class EnableEraserShortcut final : public AShortcut
{
 public:
  EnableEraserShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

}  // namespace Tactile

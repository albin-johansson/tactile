#pragma once

#include "editor/shortcuts/shortcut.hpp"

namespace Tactile {

class EnableBucketShortcut final : public AShortcut
{
 public:
  EnableBucketShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

}  // namespace Tactile

#pragma once

#include "shortcuts/shortcut.hpp"

namespace Tactile {

class EnableBucketShortcut final : public AShortcut
{
 public:
  EnableBucketShortcut();

  void Activate(entt::dispatcher& dispatcher) override;
};

}  // namespace Tactile

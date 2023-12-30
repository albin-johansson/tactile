// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/persist/settings.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Provides the high-level API to the core model.
 */
class TACTILE_CORE_API Model final {
 public:
  [[nodiscard]]
  auto settings() -> Settings&;

  [[nodiscard]]
  auto settings() const -> const Settings&;

 private:
  Settings mSettings {};
};

}  // namespace tactile

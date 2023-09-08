// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

TACTILE_FWD(class Metadata)

/** Base interface for all objects that may feature properties and components. */
class TACTILE_CORE_API IMetaContext {
 public:
  TACTILE_INTERFACE_CLASS(IMetaContext);

  [[nodiscard]] virtual auto get_meta() -> Metadata& = 0;
  [[nodiscard]] virtual auto get_meta() const -> const Metadata& = 0;
};

}  // namespace tactile

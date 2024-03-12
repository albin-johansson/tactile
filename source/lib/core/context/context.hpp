// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/context/context_visitor.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

class ContextInfo;

/// Interface for all objects that may feature properties and components.
/// Remember to update the context visitor when adding a new context implementation.
class Context {
 public:
  TACTILE_INTERFACE_CLASS(Context);

  virtual void accept(ContextVisitor& visitor) const = 0;

  /// Returns the unique identifier associated with the element.
  [[nodiscard]] virtual auto get_uuid() const -> const UUID& = 0;

  [[nodiscard]] virtual auto get_ctx() -> ContextInfo& = 0;
  [[nodiscard]] virtual auto get_ctx() const -> const ContextInfo& = 0;
};

}  // namespace tactile

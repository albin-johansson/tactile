// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/context/context.hpp"
#include "core/layer/layer_type.hpp"
#include "core/layer/layer_visitor.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/int.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

/// Interface for all layer classes.
class Layer : public Context {
 public:
  using Context::accept;
  virtual void accept(LayerVisitor& visitor) = 0;
  virtual void accept(ConstLayerVisitor& visitor) const = 0;

  /// Sets the opacity of the layer, clamped to be within [0, 1].
  virtual void set_opacity(float opacity) = 0;

  /// Sets whether the layer is rendered.
  virtual void set_visible(bool visible) = 0;

  /// Sets the parent layer ID.
  virtual void set_parent(const Maybe<UUID>& parent_id) = 0;

  /// Sets the human-readable identifier associated with the layer (used in save files).
  virtual void set_meta_id(int32 id) = 0;

  /// Returns the layer opacity, in the range [0, 1].
  [[nodiscard]] virtual auto get_opacity() const -> float = 0;

  /// Indicates whether the layer is rendered.
  [[nodiscard]] virtual auto is_visible() const -> bool = 0;

  /// Clones the layer (with a different UUID).
  [[nodiscard]] virtual auto clone() const -> Shared<Layer> = 0;

  /// Returns the type tag of the layer instance.
  [[nodiscard]] virtual auto get_type() const -> LayerType = 0;

  /// Returns the parent group layer ID, if there is one.
  [[nodiscard]] virtual auto get_parent() const -> Maybe<UUID> = 0;

  /// Returns a human-readable identifier, if there is one.
  [[nodiscard]] virtual auto get_meta_id() const -> Maybe<int32> = 0;
};

}  // namespace tactile

// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include "tactile/core/layer/layer_id.hpp"
#include "tactile/core/primitives.hpp"

namespace tactile {

/// Represents a layer of a map.
class Layer final
{
 public:
  explicit Layer(LayerId id);

  /// Sets the opacity of the layer when rendered.
  ///
  /// \param[in] opacity
  ///            The new opacity. This is clamped to the interval [0, 1].
  void set_opacity(float32 opacity);

  /// Sets whether the layer is visible.
  ///
  /// \param[in] visible
  ///            The new visibility.
  void set_visible(bool visible);

  /// Returns the unique identifier associated with the layer.
  ///
  /// \returns The layer identifier.
  auto get_id() const -> LayerId { return m_id; }

  /// Returns the opacity of the layer when rendered.
  ///
  /// \returns The layer opacity, in the interval [0, 1].
  auto get_opacity() const -> float32 { return m_opacity; }

  /// Indicates whether the layer is visible.
  ///
  /// \returns True if the layer is visible; false otherwise.
  auto is_visible() const -> bool { return m_visible; }

 private:
  LayerId m_id;
  float32 m_opacity {1.0};
  bool m_visible {true};
};

}  // namespace tactile

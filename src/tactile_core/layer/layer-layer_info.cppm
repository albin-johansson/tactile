// Copyright (C) 2025 Albin Johansson

export module tactile.layer:layer_info;

export import tactile.common;

export namespace tactile {

using LayerID = i32;

/// Provides information featured by all layer variants.
class LayerInfo final
{
 public:
  explicit LayerInfo(LayerID id);

  /// Sets the opacity of the layer when rendered.
  void set_opacity(f32 opacity);

  /// Sets whether the layer is rendered.
  void set_visible(bool visible);

  /// Returns the associated identifier.
  [[nodiscard]]
  auto id() const noexcept -> LayerID;

  /// Returns the opacity of the layer when rendered.
  [[nodiscard]]
  auto opacity() const noexcept -> f32;

  /// Indicates whether the layer is rendered.
  [[nodiscard]]
  auto visible() const noexcept -> bool;

 private:
  LayerID m_id;
  f32 m_opacity {1.0f};
  bool m_visible {true};
};

}  // namespace tactile

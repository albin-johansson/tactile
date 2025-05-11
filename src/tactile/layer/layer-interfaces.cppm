// Copyright (C) 2025 Albin Johansson

export module tactile.layer:interfaces;

export import :layer_info;

export namespace tactile {

class GroupLayer;
class TileLayer;
class AnnotationLayer;

/// Visitor interface for layer types.
///
/// Layers are visited in depth-first order.
class ILayerVisitor
{
 protected:
  ILayerVisitor() = default;

  ILayerVisitor(ILayerVisitor&&) noexcept = default;

  ILayerVisitor(const ILayerVisitor&) = default;

  auto operator=(ILayerVisitor&&) noexcept -> ILayerVisitor& = default;

  auto operator=(const ILayerVisitor&) -> ILayerVisitor& = default;

 public:
  virtual ~ILayerVisitor() noexcept = default;

  /// Called for each group layer.
  virtual void on_group_layer(GroupLayer& layer) = 0;

  /// Called for each tile layer.
  virtual void on_tile_layer(TileLayer& layer) = 0;

  /// Called for each annotation layer.
  virtual void on_annotation_layer(AnnotationLayer& layer) = 0;
};

/// Read-only visitor interface for layer types.
///
/// Layers are visited in depth-first order.
class IConstLayerVisitor
{
 protected:
  IConstLayerVisitor() = default;

  IConstLayerVisitor(IConstLayerVisitor&&) noexcept = default;

  IConstLayerVisitor(const IConstLayerVisitor&) = default;

  auto operator=(IConstLayerVisitor&&) noexcept -> IConstLayerVisitor& = default;

  auto operator=(const IConstLayerVisitor&) -> IConstLayerVisitor& = default;

 public:
  virtual ~IConstLayerVisitor() noexcept = default;

  /// Called for each group layer.
  virtual void on_group_layer(const GroupLayer& layer) = 0;

  /// Called for each tile layer.
  virtual void on_tile_layer(const TileLayer& layer) = 0;

  /// Called for each annotation layer.
  virtual void on_annotation_layer(const AnnotationLayer& layer) = 0;
};

/// Interface for layer types.
class ILayer
{
 protected:
  ILayer() = default;

  ILayer(ILayer&&) noexcept = default;

  ILayer(const ILayer&) = default;

  auto operator=(ILayer&&) noexcept -> ILayer& = default;

  auto operator=(const ILayer&) -> ILayer& = default;

 public:
  virtual ~ILayer() noexcept = default;

  /// Visits the layer.
  virtual void visit(ILayerVisitor& visitor) = 0;

  /// Visits the layer.
  virtual void visit(IConstLayerVisitor& visitor) const = 0;

  /// Returns the common layer information.
  [[nodiscard]]
  virtual auto info() -> LayerInfo& = 0;

  /// Returns the common layer information.
  [[nodiscard]]
  virtual auto info() const -> const LayerInfo& = 0;
};

}  // namespace tactile

// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

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
 public:
  virtual ~ILayer() noexcept = default;

  /// Visits the layer.
  virtual void visit(ILayerVisitor& visitor) = 0;

  /// Visits the layer.
  virtual void visit(IConstLayerVisitor& visitor) const = 0;

  /// Returns the common layer information.
  virtual auto info() -> LayerInfo& = 0;

  /// Returns the common layer information.
  virtual auto info() const -> const LayerInfo& = 0;
};

}  // namespace tactile

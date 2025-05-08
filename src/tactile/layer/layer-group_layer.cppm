// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

export module tactile.layer:group_layer;

export import :interfaces;
export import :layer_info;

export namespace tactile {

/// A layer type that provides recursive layer storage.
///
/// Complexity comments use N to denote the total number of layers in the group.
class GroupLayer final : public ILayer
{
 public:
  /// Creates an empty group layer.
  explicit GroupLayer(LayerID id);

  void visit(ILayerVisitor& visitor) override;

  void visit(IConstLayerVisitor& visitor) const override;

  [[nodiscard]]
  auto info() -> LayerInfo& override;

  [[nodiscard]]
  auto info() const -> const LayerInfo& override;

  /// Appends a layer to the group.
  ///
  /// Complexity: O(N)
  void append_layer(Unique<ILayer> layer);

  /// Appends a layer to a nested group.
  ///
  /// Complexity: O(N)
  void append_layer_to(LayerID parent_id, Unique<ILayer> layer);

  /// Removes a layer from the group.
  ///
  /// Complexity: O(N)
  auto remove_layer(LayerID id) -> Unique<ILayer>;

  /// Raises a layer relative to its sibling layers.
  ///
  /// Complexity: O(N)
  [[nodiscard]]
  auto raise_layer(LayerID id) -> Result<void>;

  /// Lowers a layer relative to its sibling layers.
  ///
  /// Complexity: O(N)
  [[nodiscard]]
  auto lower_layer(LayerID id) -> Result<void>;

  /// Returns the relative index of a layer.
  ///
  /// Complexity: O(N)
  [[nodiscard]]
  auto layer_index_rel(LayerID id) const -> usize;

  /// Returns the absolute index of a layer.
  ///
  /// Complexity: O(N)
  [[nodiscard]]
  auto layer_index_abs(LayerID id) const -> usize;

  /// Recursively searches for a layer with a given ID.
  ///
  /// Complexity: O(N)
  [[nodiscard]]
  auto find_layer(LayerID id) -> ILayer*;

  /// Recursively searches for a layer with a given ID.
  ///
  /// Complexity: O(N)
  [[nodiscard]]
  auto find_layer(LayerID id) const -> const ILayer*;

  /// Recursively searches for the parent of a layer with a given ID.
  ///
  /// Complexity: O(N)
  [[nodiscard]]
  auto find_parent_layer(LayerID id) -> GroupLayer*;

  /// Recursively searches for the parent of a layer with a given ID.
  ///
  /// Complexity: O(N)
  [[nodiscard]]
  auto find_parent_layer(LayerID id) const -> const GroupLayer*;

  /// Recursively counts the number of descendants of this group.
  ///
  /// Complexity: O(N)
  [[nodiscard]]
  auto layer_count() const -> usize;

 private:
  LayerInfo m_info;
  Vector<Unique<ILayer>> m_layers {};

  struct FindLayerResult final
  {
    GroupLayer* parent_layer;
    usize rel_index;
    usize abs_index;
    bool found;
  };

  struct FindConstLayerResult final
  {
    const GroupLayer* parent_layer;
    usize rel_index;
    usize abs_index;
    bool found;
  };

  /// Complexity: O(N)
  [[nodiscard]]
  auto _find_layer(LayerID id, usize abs_index = 0) -> FindLayerResult;

  /// Complexity: O(N)
  [[nodiscard]]
  auto _find_layer(LayerID id, usize abs_index = 0) const -> FindConstLayerResult;
};

}  // namespace tactile

// Copyright (C) 2025 Albin Johansson

export module tactile.core.layer:annotation_layer;

import :interfaces;
import :layer_info;
import tactile.core.prelude;
import tactile.core.math;

export namespace tactile {

/// Type used for annotation object identifiers.
using AnnotationID = i32;

/// Represents different kinds of annotation objects.
enum class AnnotationKind : u8
{
  kPoint,
  kRect,
  kEllipse,
};

/// Describes an annotation in an annotation layer.
struct Annotation final
{
  /// The unique identifier associated with the annotation.
  AnnotationID id;

  /// The type of the annotation.
  AnnotationKind kind;

  /// The logical position.
  Float2 position;

  /// The logical size.
  Float2 size;

  /// The user-defined name.
  String name;

  /// Indicates whether the annotation is rendered.
  bool visible;
};

/// Creates an annotation object.
[[nodiscard]]
auto make_annotation(AnnotationID id, AnnotationKind kind)
    -> Unique<Annotation>;

/// A layer variant consisting of zero or more annotations.
class AnnotationLayer final : public ILayer
{
 public:
  using storage_type = Vector<Unique<Annotation>>;
  using const_iterator = storage_type::const_iterator;

  /// Creates an empty annotation layer.
  explicit AnnotationLayer(LayerID id);

  void visit(ILayerVisitor& visitor) override;

  void visit(IConstLayerVisitor& visitor) const override;

  [[nodiscard]]
  auto info() -> LayerInfo& override;

  [[nodiscard]]
  auto info() const -> const LayerInfo& override;

  /// Adds an annotation to the layer.
  ///
  /// Complexity: O(1)
  void add_annotation(Unique<Annotation> annotation);

  /// Removes an annotation from the layer.
  ///
  /// Complexity: O(N)
  auto remove_annotation(AnnotationID id) -> Unique<Annotation>;

  /// Searches for an annotation with a given identifier.
  ///
  /// Complexity: O(N)
  [[nodiscard]]
  auto find_annotation(AnnotationID id) -> Annotation*;

  /// Searches for an annotation with a given identifier.
  ///
  /// Complexity: O(N)
  [[nodiscard]]
  auto find_annotation(AnnotationID id) const -> const Annotation*;

  /// Returns the annotation at a given index.
  ///
  /// Complexity: O(1)
  [[nodiscard]]
  auto annotation_at(usize index) -> Annotation&;

  /// Returns the annotation at a given index.
  ///
  /// Complexity: O(1)
  [[nodiscard]]
  auto annotation_at(usize index) const -> const Annotation&;

  /// Returns the number of annotations in the layer.
  ///
  /// Complexity: O(1)
  [[nodiscard]]
  auto annotation_count() const -> usize;

  /// Returns an iterator to the beginning of the layer.
  [[nodiscard]]
  auto begin() const -> const_iterator;

  /// Returns an iterator to the end of the layer.
  [[nodiscard]]
  auto end() const -> const_iterator;

 private:
  LayerInfo m_info;
  storage_type m_annotations {};
};

}  // namespace tactile

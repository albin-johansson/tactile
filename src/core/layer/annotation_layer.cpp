// Copyright (C) 2025 Albin Johansson

module tactile.core.layer;

import std;

namespace tactile {

auto make_annotation(const AnnotationID id, const AnnotationKind kind)
    -> Unique<Annotation>
{
  auto annotation = make_unique<Annotation>();

  annotation->id = id;
  annotation->kind = kind;
  annotation->position = Float2 {0, 0};
  annotation->size = Float2 {0, 0};
  annotation->visible = true;

  return annotation;
}

AnnotationLayer::AnnotationLayer(const LayerID id)
  : m_info {id}
{}

void AnnotationLayer::visit(ILayerVisitor& visitor)
{
  visitor.on_annotation_layer(*this);
}

void AnnotationLayer::visit(IConstLayerVisitor& visitor) const
{
  visitor.on_annotation_layer(*this);
}

auto AnnotationLayer::info() -> LayerInfo&
{
  return m_info;
}

auto AnnotationLayer::info() const -> const LayerInfo&
{
  return m_info;
}

void AnnotationLayer::add_annotation(Unique<Annotation> annotation)
{
  if (annotation == nullptr) {
    throw std::invalid_argument {"tried to add null annotation"};
  }

  m_annotations.push_back(std::move(annotation));
}

auto AnnotationLayer::remove_annotation(const AnnotationID id) -> Unique<Annotation>
{
  const auto iter = std::ranges::find_if(
      m_annotations,
      [id](const Unique<Annotation>& annotation) { return annotation->id == id; });

  if (iter == m_annotations.end()) {
    return nullptr;
  }

  auto removed_annotation = std::move(*iter);
  m_annotations.erase(iter);

  return removed_annotation;
}

auto AnnotationLayer::annotation_at(const usize index) -> Annotation&
{
  return *m_annotations.at(index);
}

auto AnnotationLayer::annotation_at(const usize index) const -> const Annotation&
{
  return *m_annotations.at(index);
}

auto AnnotationLayer::find_annotation(const AnnotationID id) -> Annotation*
{
  const auto iter = std::ranges::find_if(
      m_annotations,
      [id](const Unique<Annotation>& annotation) { return annotation->id == id; });
  return iter == m_annotations.end() ? nullptr : iter->get();
}

auto AnnotationLayer::find_annotation(const AnnotationID id) const
    -> const Annotation*
{
  const auto iter = std::ranges::find_if(
      m_annotations,
      [id](const Unique<Annotation>& annotation) { return annotation->id == id; });
  return iter == m_annotations.end() ? nullptr : iter->get();
}

auto AnnotationLayer::annotation_count() const -> usize
{
  return m_annotations.size();
}

auto AnnotationLayer::begin() const -> const_iterator
{
  return m_annotations.begin();
}

auto AnnotationLayer::end() const -> const_iterator
{
  return m_annotations.end();
}

}  // namespace tactile

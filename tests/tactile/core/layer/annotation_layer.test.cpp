// Copyright (C) 2025 Albin Johansson

#include <gmock/gmock.h>
#include <gtest/gtest.h>

import tactile.core.ext.std;
import tactile.core.layer;

namespace tactile::tests {
namespace {

struct TestAnnotations final
{
  AnnotationID point_id;
  AnnotationID rect_id;
  AnnotationID ellipse_id;
  AnnotationID invalid_id;
};

class AnnotationLayerTest : public testing::Test
{
 protected:
  AnnotationLayer m_layer {LayerID {42}};
  AnnotationID m_next_annotation_id {1};

  [[nodiscard]]
  auto prepare_annotations() -> TestAnnotations
  {
    TestAnnotations annotations {};
    annotations.point_id = m_next_annotation_id++;
    annotations.rect_id = m_next_annotation_id++;
    annotations.ellipse_id = m_next_annotation_id++;
    annotations.invalid_id = AnnotationID {100};

    m_layer.add_annotation(
        make_annotation(annotations.point_id, AnnotationKind::kPoint));
    m_layer.add_annotation(
        make_annotation(annotations.rect_id, AnnotationKind::kRect));
    m_layer.add_annotation(
        make_annotation(annotations.ellipse_id, AnnotationKind::kEllipse));

    return annotations;
  }
};

TEST_F(AnnotationLayerTest, AddAnnotation)
{
  const auto annotation_id = m_next_annotation_id++;
  ASSERT_EQ(m_layer.annotation_count(), 0uz);
  EXPECT_EQ(m_layer.find_annotation(annotation_id), nullptr);

  auto annotation = make_annotation(annotation_id, AnnotationKind::kPoint);
  m_layer.add_annotation(std::move(annotation));

  EXPECT_EQ(m_layer.annotation_count(), 1uz);
  EXPECT_NE(m_layer.find_annotation(annotation_id), nullptr);

  EXPECT_THROW(m_layer.add_annotation(nullptr), std::invalid_argument);
}

TEST_F(AnnotationLayerTest, RemoveAnnotation)
{
  const auto annotations = prepare_annotations();
  ASSERT_EQ(m_layer.annotation_count(), 3uz);

  const auto removed_annotation =
      m_layer.remove_annotation(annotations.point_id);
  ASSERT_NE(removed_annotation, nullptr);
  EXPECT_EQ(removed_annotation->id, annotations.point_id);
  EXPECT_EQ(m_layer.annotation_count(), 2uz);

  EXPECT_EQ(m_layer.remove_annotation(annotations.invalid_id), nullptr);
  EXPECT_EQ(m_layer.annotation_count(), 2uz);
}

TEST_F(AnnotationLayerTest, FindAnnotation)
{
  const auto annotations = prepare_annotations();

  EXPECT_NE(m_layer.find_annotation(annotations.point_id), nullptr);
  EXPECT_NE(m_layer.find_annotation(annotations.rect_id), nullptr);
  EXPECT_NE(m_layer.find_annotation(annotations.ellipse_id), nullptr);
  EXPECT_EQ(m_layer.find_annotation(annotations.invalid_id), nullptr);

  EXPECT_NE(std::as_const(m_layer).find_annotation(annotations.point_id),
            nullptr);
  EXPECT_NE(std::as_const(m_layer).find_annotation(annotations.rect_id),
            nullptr);
  EXPECT_NE(std::as_const(m_layer).find_annotation(annotations.ellipse_id),
            nullptr);
  EXPECT_EQ(std::as_const(m_layer).find_annotation(annotations.invalid_id),
            nullptr);
}

TEST_F(AnnotationLayerTest, AnnotationAt)
{
  const auto annotations = prepare_annotations();

  EXPECT_EQ(m_layer.annotation_at(0uz).id, annotations.point_id);
  EXPECT_EQ(m_layer.annotation_at(1uz).id, annotations.rect_id);
  EXPECT_EQ(m_layer.annotation_at(2uz).id, annotations.ellipse_id);

  EXPECT_EQ(std::as_const(m_layer).annotation_at(0uz).id, annotations.point_id);
  EXPECT_EQ(std::as_const(m_layer).annotation_at(1uz).id, annotations.rect_id);
  EXPECT_EQ(std::as_const(m_layer).annotation_at(2uz).id,
            annotations.ellipse_id);

  EXPECT_THROW((void) m_layer.annotation_at(3uz), std::out_of_range);
  EXPECT_THROW((void) std::as_const(m_layer).annotation_at(3uz),
               std::out_of_range);
}

TEST_F(AnnotationLayerTest, AnnotationCount)
{
  EXPECT_EQ(m_layer.annotation_count(), 0uz);

  (void) prepare_annotations();

  EXPECT_EQ(m_layer.annotation_count(), 3uz);
}

TEST_F(AnnotationLayerTest, BeginEnd)
{
  EXPECT_EQ(std::distance(m_layer.begin(), m_layer.end()), 0z);

  (void) prepare_annotations();

  EXPECT_EQ(std::distance(m_layer.begin(), m_layer.end()), 3z);
}

}  // namespace
}  // namespace tactile::tests

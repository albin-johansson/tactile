#pragma once

#include "mouse_tool.hpp"

namespace tactile {

/**
 * \class BucketTool
 *
 * \brief Represents a "bucket" tool, which uses a flood-fill algorithm to
 * set tiles.
 *
 * \since 0.1.0
 *
 * \headerfile bucket_tool.hpp
 */
class BucketTool final : public AMouseTool
{
 public:
  /**
   * \brief Creates a `bucket_tool` instance.
   *
   * \param model a pointer to the associated model, cannot be null.
   *
   * \throws tactile_error if the supplied pointer is null.
   *
   * \since 0.1.0
   */
  explicit BucketTool(core::Model* model);

  void OnPressed(QMouseEvent* event, const QPointF& mapPosition) override;
};

}  // namespace tactile

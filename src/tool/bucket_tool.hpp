#pragma once

#include "abstract_tool.hpp"

namespace tactile {

/**
 * \class bucket_tool
 *
 * \brief Represents a "bucket" tool, which uses a flood-fill algorithm to
 * set tiles.
 *
 * \since 0.1.0
 *
 * \headerfile bucket_tool.hpp
 */
class bucket_tool final : public abstract_tool
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
  explicit bucket_tool(core::model* model);

  void pressed(QMouseEvent* event, const QPointF& mapPosition) override;
};

}  // namespace tactile

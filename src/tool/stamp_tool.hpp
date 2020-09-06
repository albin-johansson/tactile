#pragma once

#include "fwd.hpp"
#include "tool.hpp"

namespace tactile {

class stamp_tool final : public tool
{
 public:
  explicit stamp_tool(core::model* model);

  void pressed(QMouseEvent* event, const QPointF& mapPosition) override;

  void moved(QMouseEvent* event, const QPointF& mapPosition) override;

  void released(QMouseEvent* event, const QPointF& mapPosition) override;

 private:
  core::model* m_model{};

  //  [[nodiscard]] auto is_single_tile_selected() const -> bool;
};

}  // namespace tactile

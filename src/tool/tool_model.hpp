#pragma once

#include <memory>  // unique_ptr

#include "abstract_tool.hpp"
#include "tool_id.hpp"

namespace tactile {

class tool_model final
{
 public:
  explicit tool_model(core::model* model);

  void select(tool_id tool);

  void pressed(QMouseEvent* event, const QPointF& mapPosition);

  void moved(QMouseEvent* event, const QPointF& mapPosition);

  void released(QMouseEvent* event, const QPointF& mapPosition);

  void entered(QEvent* event);

  void exited(QEvent* event);

 private:
  abstract_tool* m_current{};
  std::unique_ptr<abstract_tool> m_stamp;
  std::unique_ptr<abstract_tool> m_eraser;

  void switch_to(abstract_tool* tool);
};

}  // namespace tactile

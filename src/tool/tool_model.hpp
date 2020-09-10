#pragma once

#include <memory>  // unique_ptr

#include "fwd.hpp"
#include "tool.hpp"

namespace tactile {

class tool_model final : public tool
{
 public:
  enum class tool_id { none, stamp };

  explicit tool_model(core::model* model);

  void select(tool_id tool);

  void pressed(QMouseEvent* event, const QPointF& mapPosition) override;

  void moved(QMouseEvent* event, const QPointF& mapPosition) override;

  void released(QMouseEvent* event, const QPointF& mapPosition) override;

  void entered(QEvent* event) override;

  void exited(QEvent* event) override;

 private:
  // TODO avoid virtual inheritance because all tools are known at compile-time
  tool* m_current{};
  std::unique_ptr<tool> m_stamp;
};

}  // namespace tactile

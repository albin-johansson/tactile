#pragma once

#include <qevent.h>

namespace tactile {

class tool
{
 public:
  virtual ~tool() noexcept = default;

  virtual void pressed(QMouseEvent* event, const QPointF& mapPosition) = 0;

  virtual void moved(QMouseEvent* event, const QPointF& mapPosition) = 0;

  virtual void released(QMouseEvent* event, const QPointF& mapPosition) = 0;

  virtual void entered(QEvent* event) = 0;

  virtual void exited(QEvent* event) = 0;
};

}  // namespace tactile

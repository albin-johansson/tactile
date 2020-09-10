#include "tool_model.hpp"

#include "stamp_tool.hpp"

namespace tactile {

tool_model::tool_model(core::model* model)
    : m_stamp{std::make_unique<stamp_tool>(model)}
{
  // FIXME only for testing
  m_current = m_stamp.get();
}

void tool_model::select(tool_id tool)
{
  switch (tool) {
    default:
      [[fallthrough]];
    case tool_id::none: {
      m_current = nullptr;
      break;
    }
    case tool_id::stamp: {
      m_current = m_stamp.get();
      break;
    }
  }
}

void tool_model::pressed(QMouseEvent* event, const QPointF& mapPosition)
{
  if (m_current) {
    m_current->pressed(event, mapPosition);
  }
}

void tool_model::moved(QMouseEvent* event, const QPointF& mapPosition)
{
  if (m_current) {
    m_current->moved(event, mapPosition);
  }
}

void tool_model::released(QMouseEvent* event, const QPointF& mapPosition)
{
  if (m_current) {
    m_current->released(event, mapPosition);
  }
}
void tool_model::entered(QEvent* event)
{
  if (m_current) {
    m_current->entered(event);
  }
}

void tool_model::exited(QEvent* event)
{
  if (m_current) {
    m_current->exited(event);
  }
}

}  // namespace tactile

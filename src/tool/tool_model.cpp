#include "tool_model.hpp"

#include "bucket_tool.hpp"
#include "eraser_tool.hpp"
#include "stamp_tool.hpp"

namespace tactile {

tool_model::tool_model(core::model* model)
    : m_stamp{std::make_unique<stamp_tool>(model)},
      m_eraser{std::make_unique<eraser_tool>(model)},
      m_bucket{std::make_unique<bucket_tool>(model)}
{}

void tool_model::switch_to(abstract_tool* tool)
{
  if (m_current == tool) {
    return;
  }

  if (m_current) {
    m_current->disable();
  }

  m_current = tool;

  if (m_current) {
    m_current->enable();
  }
}

void tool_model::select(tool_id tool)
{
  switch (tool) {  // TODO rectangle, find_same
    default:
      [[fallthrough]];
    case tool_id::none: {
      switch_to(nullptr);
      break;
    }
    case tool_id::stamp: {
      switch_to(m_stamp.get());
      break;
    }
    case tool_id::eraser: {
      switch_to(m_eraser.get());
      break;
    }
    case tool_id::bucket: {
      switch_to(m_bucket.get());
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

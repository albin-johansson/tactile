#include "mouse_tool_model.hpp"

#include "bucket_tool.hpp"
#include "eraser_tool.hpp"
#include "model.hpp"
#include "stamp_tool.hpp"

namespace tactile {

MouseToolModel::MouseToolModel(core::Model* model)
{
  mTools.emplace(MouseToolType::None, nullptr);
  mTools.emplace(MouseToolType::Stamp, std::make_unique<StampTool>(model));
  mTools.emplace(MouseToolType::Eraser, std::make_unique<EraserTool>(model));
  mTools.emplace(MouseToolType::Bucket, std::make_unique<BucketTool>(model));
}

void MouseToolModel::SwitchTo(AMouseTool* tool)
{
  if (mCurrent == tool)
  {
    return;
  }

  if (mCurrent)
  {
    mCurrent->Disable();
  }

  mCurrent = tool;

  if (mCurrent)
  {
    mCurrent->Enable();
  }
}

void MouseToolModel::Select(const MouseToolType id)
{
  SwitchTo(mTools.at(id).get());
}

void MouseToolModel::OnMousePressed(QMouseEvent* event, const QPointF& mapPosition)
{
  if (mCurrent)
  {
    mCurrent->OnPressed(event, mapPosition);
  }
}

void MouseToolModel::OnMouseMoved(QMouseEvent* event, const QPointF& mapPosition)
{
  if (mCurrent)
  {
    mCurrent->OnMoved(event, mapPosition);
  }
}

void MouseToolModel::OnMouseReleased(QMouseEvent* event, const QPointF& mapPosition)
{
  if (mCurrent)
  {
    mCurrent->OnReleased(event, mapPosition);
  }
}

void MouseToolModel::OnMouseEntered(QEvent* event)
{
  if (mCurrent)
  {
    mCurrent->OnEntered(event);
  }
}

void MouseToolModel::OnMouseExited(QEvent* event)
{
  if (mCurrent)
  {
    mCurrent->OnExited(event);
  }
}

}  // namespace tactile

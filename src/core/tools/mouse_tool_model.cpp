#include "mouse_tool_model.hpp"

#include "bucket_tool.hpp"
#include "eraser_tool.hpp"
#include "stamp_tool.hpp"

namespace Tactile {

MouseToolModel::MouseToolModel(NotNull<Model*> model)
{
  mTools.emplace(MouseToolType::None, nullptr);
  mTools.emplace(MouseToolType::Stamp, std::make_unique<StampTool>(model));
  mTools.emplace(MouseToolType::Eraser, std::make_unique<EraserTool>(model));
  mTools.emplace(MouseToolType::Bucket, std::make_unique<BucketTool>(model));
}

void MouseToolModel::Select(const MouseToolType type)
{
  SwitchTo(mTools.at(type).get());
}

void MouseToolModel::OnMousePressed(const MousePressedEvent& event)
{
  if (auto* tool = mCurrent)
  {
    tool->OnPressed(event.info);
  }
}

void MouseToolModel::OnMouseReleased(const MouseReleasedEvent& event)
{
  if (auto* tool = mCurrent)
  {
    tool->OnReleased(event.info);
  }
}

void MouseToolModel::OnMouseDragged(const MouseDragEvent& event)
{
  if (auto* tool = mCurrent)
  {
    tool->OnDragged(event.info);
  }
}

auto MouseToolModel::GetActive() const -> MouseToolType
{
  if (mCurrent)
  {
    return mCurrent->GetType();
  }
  else
  {
    return MouseToolType::None;
  }
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

}  // namespace Tactile

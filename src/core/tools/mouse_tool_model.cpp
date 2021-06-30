#include "mouse_tool_model.hpp"

#include "bucket_tool.hpp"
#include "eraser_tool.hpp"
#include "stamp_tool.hpp"

namespace Tactile {

MouseToolModel::MouseToolModel(Model* model)
{
  mTools.emplace(MouseToolType::None, nullptr);
  mTools.emplace(MouseToolType::Stamp, std::make_unique<StampTool>());
  mTools.emplace(MouseToolType::Eraser, std::make_unique<EraserTool>());
  mTools.emplace(MouseToolType::Bucket, std::make_unique<BucketTool>());
}

void MouseToolModel::Select(const MouseToolType type)
{
  SwitchTo(mTools.at(type).get());
}

void MouseToolModel::OnMouseDragged(const MouseDragEvent& event)
{
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

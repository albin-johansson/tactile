// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tool_manager.hpp"

#include "model/tool/bucket_tool.hpp"
#include "model/tool/ellipse_tool.hpp"
#include "model/tool/eraser_tool.hpp"
#include "model/tool/object_selection_tool.hpp"
#include "model/tool/point_tool.hpp"
#include "model/tool/rectangle_tool.hpp"
#include "model/tool/stamp_tool.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile {

struct ToolManager::Data final {
  StampTool stamp;
  EraserTool eraser;
  BucketTool bucket;
  RectangleTool rectangle;
  EllipseTool ellipse;
  PointTool point;
  ObjectSelectionTool object_selection;

  Tool* active_tool {};
};

TACTILE_DEFINE_MOVE(ToolManager);

ToolManager::ToolManager()
    : mData {std::make_unique<Data>()}
{
}

ToolManager::~ToolManager() noexcept = default;

void ToolManager::accept(ToolVisitor& visitor) const
{
  if (auto* tool = mData->active_tool) {
    tool->accept(visitor);
  }
}

void ToolManager::select_tool(const ToolType type,
                              DocumentModel& model,
                              entt::dispatcher& dispatcher)
{
  if (auto* tool = mData->active_tool) {
    tool->on_disabled(model, dispatcher);
  }

  // Selecting an already active tool disables it
  if (mData->active_tool && mData->active_tool->get_type() == type) {
    mData->active_tool = nullptr;
  }
  else {
    switch (type) {
      case ToolType::None:
        mData->active_tool = nullptr;
        break;

      case ToolType::Stamp:
        mData->active_tool = &mData->stamp;
        break;

      case ToolType::Eraser:
        mData->active_tool = &mData->eraser;
        break;

      case ToolType::Bucket:
        mData->active_tool = &mData->bucket;
        break;

      case ToolType::ObjectSelection:
        mData->active_tool = &mData->object_selection;
        break;

      case ToolType::Rectangle:
        mData->active_tool = &mData->rectangle;
        break;

      case ToolType::Ellipse:
        mData->active_tool = &mData->ellipse;
        break;

      case ToolType::Point:
        mData->active_tool = &mData->point;
        break;
    }
  }

  if (auto* tool = mData->active_tool) {
    tool->on_enabled(model, dispatcher);
  }
}

void ToolManager::set_stamp_random_mode(const bool random)
{
  mData->stamp.set_random(random);
}

auto ToolManager::is_enabled(const ToolType type) const -> bool
{
  if (auto* tool = mData->active_tool) {
    return tool->get_type() == type;
  }
  else {
    return type == ToolType::None;
  }
}

auto ToolManager::is_stamp_random() const -> bool
{
  return mData->stamp.is_random();
}

auto ToolManager::is_available(const DocumentModel& model, const ToolType type) const
    -> bool
{
  switch (type) {
    case ToolType::None:
      return false;

    case ToolType::Stamp:
      return mData->stamp.is_available(model);

    case ToolType::Eraser:
      return mData->eraser.is_available(model);

    case ToolType::Bucket:
      return mData->bucket.is_available(model);

    case ToolType::ObjectSelection:
      return mData->object_selection.is_available(model);

    case ToolType::Rectangle:
      return mData->rectangle.is_available(model);

    case ToolType::Ellipse:
      return mData->ellipse.is_available(model);

    case ToolType::Point:
      return mData->point.is_available(model);

    default:
      throw Exception {"Invalid tool type!"};
  }
}

void ToolManager::on_enabled(DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (auto* tool = mData->active_tool) {
    tool->on_enabled(model, dispatcher);
  }
}

void ToolManager::on_disabled(DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (auto* tool = mData->active_tool) {
    tool->on_disabled(model, dispatcher);
  }
}

void ToolManager::on_entered(DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (auto* tool = mData->active_tool) {
    tool->on_entered(model, dispatcher);
  }
}

void ToolManager::on_exited(DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (auto* tool = mData->active_tool) {
    tool->on_exited(model, dispatcher);
  }
}

void ToolManager::on_pressed(DocumentModel& model,
                             entt::dispatcher& dispatcher,
                             const MouseInfo& mouse)
{
  if (auto* tool = mData->active_tool) {
    tool->on_pressed(model, dispatcher, mouse);
  }
}

void ToolManager::on_dragged(DocumentModel& model,
                             entt::dispatcher& dispatcher,
                             const MouseInfo& mouse)
{
  if (auto* tool = mData->active_tool) {
    tool->on_dragged(model, dispatcher, mouse);
  }
}

void ToolManager::on_released(DocumentModel& model,
                              entt::dispatcher& dispatcher,
                              const MouseInfo& mouse)
{
  if (auto* tool = mData->active_tool) {
    tool->on_released(model, dispatcher, mouse);
  }
}

auto ToolManager::get_type() const -> ToolType
{
  throw Exception {"Invalid call!"};
}

auto ToolManager::is_available(const DocumentModel&) const -> bool
{
  throw Exception {"Invalid call!"};
}

}  // namespace tactile

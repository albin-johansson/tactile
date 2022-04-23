/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "tool_manager.hpp"

#include "core/tools/bucket_tool.hpp"
#include "core/tools/ellipse_tool.hpp"
#include "core/tools/eraser_tool.hpp"
#include "core/tools/object_selection_tool.hpp"
#include "core/tools/point_tool.hpp"
#include "core/tools/rectangle_tool.hpp"
#include "core/tools/stamp_tool.hpp"
#include "misc/throw.hpp"

namespace tactile {

struct ToolManager::Data final
{
  StampTool stamp;
  EraserTool eraser;
  BucketTool bucket;
  RectangleTool rectangle;
  EllipseTool ellipse;
  PointTool point;
  ObjectSelectionTool object_selection;

  ATool* active_tool{};
};

TACTILE_DEFINE_MOVE(ToolManager);

ToolManager::ToolManager() : mData{std::make_unique<Data>()} {}

ToolManager::~ToolManager() noexcept = default;

void ToolManager::select_tool(const ToolType type,
                              entt::registry& registry,
                              entt::dispatcher& dispatcher)
{
  if (auto* tool = mData->active_tool) {
    tool->on_disabled(registry, dispatcher);
  }

  /* Selecting an already active tool disables it */
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
    tool->on_enabled(registry, dispatcher);
  }
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

void ToolManager::draw_gizmos(const entt::registry& registry,
                              IRenderer& renderer,
                              const MouseInfo& mouse) const
{
  if (auto* tool = mData->active_tool) {
    tool->draw_gizmos(registry, renderer, mouse);
  }
}

void ToolManager::on_enabled(entt::registry& registry, entt::dispatcher& dispatcher)
{
  if (auto* tool = mData->active_tool) {
    tool->on_enabled(registry, dispatcher);
  }
}

void ToolManager::on_disabled(entt::registry& registry, entt::dispatcher& dispatcher)
{
  if (auto* tool = mData->active_tool) {
    tool->on_disabled(registry, dispatcher);
  }
}

void ToolManager::on_entered(entt::registry& registry, entt::dispatcher& dispatcher)
{
  if (auto* tool = mData->active_tool) {
    tool->on_entered(registry, dispatcher);
  }
}

void ToolManager::on_exited(entt::registry& registry, entt::dispatcher& dispatcher)
{
  if (auto* tool = mData->active_tool) {
    tool->on_exited(registry, dispatcher);
  }
}

void ToolManager::on_pressed(entt::registry& registry,
                             entt::dispatcher& dispatcher,
                             const MouseInfo& mouse)
{
  if (auto* tool = mData->active_tool) {
    tool->on_pressed(registry, dispatcher, mouse);
  }
}

void ToolManager::on_dragged(entt::registry& registry,
                             entt::dispatcher& dispatcher,
                             const MouseInfo& mouse)
{
  if (auto* tool = mData->active_tool) {
    tool->on_dragged(registry, dispatcher, mouse);
  }
}

void ToolManager::on_released(entt::registry& registry,
                              entt::dispatcher& dispatcher,
                              const MouseInfo& mouse)
{
  if (auto* tool = mData->active_tool) {
    tool->on_released(registry, dispatcher, mouse);
  }
}

auto ToolManager::get_type() const -> ToolType
{
  panic("Invalid call!");
}

}  // namespace tactile
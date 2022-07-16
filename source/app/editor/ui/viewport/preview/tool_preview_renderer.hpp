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

#pragma once

#include "core/common/functional.hpp"
#include "core/fwd.hpp"
#include "core/mouse_info.hpp"
#include "core/tool/tool_visitor.hpp"

namespace tactile::ui {

class GraphicsCtx;

/// Handles the in-viewport previews for all relevant mouse tools.
class ToolPreviewRenderer final : public IToolVisitor
{
 public:
  explicit ToolPreviewRenderer(const DocumentModel& model,
                               GraphicsCtx&         graphics,
                               const MouseInfo&     mouse);

  void visit(const StampTool& tool) override;

  void visit(const RectangleTool& tool) override;

  void visit(const EllipseTool& tool) override;

 private:
  Ref<const DocumentModel> mModel;
  Ref<GraphicsCtx>         mGraphics;
  MouseInfo                mMouseInfo;

  void render_stamp_normal(const Map& map, const TilesetRef& tilesetRef);
};

}  // namespace tactile::ui

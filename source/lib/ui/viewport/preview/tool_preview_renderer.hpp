/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "model/mouse_info.hpp"
#include "model/tool/tool_visitor.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/type/ref.hpp"

TACTILE_FWD_NS(tactile, class DocumentModel)
TACTILE_FWD_NS(tactile, class Map)
TACTILE_FWD_NS(tactile, class TilesetRef)
TACTILE_FWD_NS(tactile, class Renderer)

namespace tactile::ui {

/// Handles the in-viewport previews for all relevant mouse tools.
class ToolPreviewRenderer final : public ToolVisitor {
 public:
  explicit ToolPreviewRenderer(const DocumentModel& model,
                               const Renderer& renderer,
                               const MouseInfo& mouse);

  void visit(const StampTool& tool) override;

  void visit(const RectangleTool& tool) override;

  void visit(const EllipseTool& tool) override;

 private:
  Ref<const DocumentModel> mModel;
  Ref<const Renderer> mRenderer;
  MouseInfo mMouseInfo;

  void render_stamp_normal(const Map& map, const TilesetRef& tileset_ref);
};

}  // namespace tactile::ui

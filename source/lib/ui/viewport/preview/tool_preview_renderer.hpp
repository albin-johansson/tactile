// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "model/mouse_info.hpp"
#include "model/tool/tool_visitor.hpp"
#include "tactile/base/container/ref.hpp"
#include "tactile/base/prelude.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, DocumentModel)
TACTILE_FWD_DECLARE_CLASS_NS(tactile, Map)
TACTILE_FWD_DECLARE_CLASS_NS(tactile, TilesetRef)
TACTILE_FWD_DECLARE_CLASS_NS(tactile, Renderer)

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

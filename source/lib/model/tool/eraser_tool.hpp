// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "common/type/tile_cache.hpp"
#include "model/tool/tool.hpp"

namespace tactile {

class EraserTool final : public Tool {
 public:
  void accept(ToolVisitor& visitor) const override;

  void on_disabled(DocumentModel& model, entt::dispatcher& dispatcher) override;

  void on_exited(DocumentModel& model, entt::dispatcher& dispatcher) override;

  void on_pressed(DocumentModel& model,
                  entt::dispatcher& dispatcher,
                  const MouseInfo& mouse) override;

  void on_dragged(DocumentModel& model,
                  entt::dispatcher& dispatcher,
                  const MouseInfo& mouse) override;

  void on_released(DocumentModel& model,
                   entt::dispatcher& dispatcher,
                   const MouseInfo& mouse) override;

  [[nodiscard]] auto is_available(const DocumentModel& model) const -> bool override;

  [[nodiscard]] auto get_type() const -> ToolType override { return ToolType::Eraser; }

 private:
  TileCache mPrevState;

  void update_sequence(DocumentModel& model, const TilePos& cursor);

  void maybe_emit_event(const DocumentModel& model, entt::dispatcher& dispatcher);
};

}  // namespace tactile

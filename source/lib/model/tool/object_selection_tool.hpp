// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "model/tool/tool.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

struct ObjectDragInfo final {
  Float2 origin_object_pos {};  ///< Object position at drag start.
  Float2 last_mouse_pos {};     ///< Mouse viewport position at last update.
};

class ObjectSelectionTool final : public Tool {
 public:
  void accept(ToolVisitor& visitor) const override;

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

  [[nodiscard]] auto get_type() const -> ToolType override
  {
    return ToolType::ObjectSelection;
  }

 private:
  Maybe<ObjectDragInfo> mDragInfo;

  void maybe_emit_event(DocumentModel& model, entt::dispatcher& dispatcher);
};

}  // namespace tactile

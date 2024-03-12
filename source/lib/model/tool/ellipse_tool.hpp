// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "model/tool/tool.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

struct CurrentEllipseStroke final {
  Float2 start {};
  Float2 current {};
};

class EllipseTool final : public Tool {
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

  [[nodiscard]] auto get_stroke() const -> const Maybe<CurrentEllipseStroke>&;

  [[nodiscard]] auto get_type() const -> ToolType override { return ToolType::Ellipse; }

 private:
  Maybe<CurrentEllipseStroke> mStroke;

  void maybe_emit_event(DocumentModel& model, entt::dispatcher& dispatcher);
};

}  // namespace tactile

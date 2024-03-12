// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <entt/fwd.hpp>

#include "model/tool/tool_type.hpp"
#include "model/tool/tool_visitor.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_STRUCT(MouseInfo)
TACTILE_FWD_DECLARE_CLASS(DocumentModel)

class Tool {
 public:
  virtual ~Tool() noexcept = default;

  virtual void accept(ToolVisitor& visitor) const = 0;

  virtual void on_enabled(DocumentModel& model, entt::dispatcher& dispatcher);

  virtual void on_disabled(DocumentModel& model, entt::dispatcher& dispatcher);

  virtual void on_entered(DocumentModel& model, entt::dispatcher& dispatcher);

  virtual void on_exited(DocumentModel& model, entt::dispatcher& dispatcher);

  virtual void on_pressed(DocumentModel& model,
                          entt::dispatcher& dispatcher,
                          const MouseInfo& mouse);

  virtual void on_dragged(DocumentModel& model,
                          entt::dispatcher& dispatcher,
                          const MouseInfo& mouse);

  virtual void on_released(DocumentModel& model,
                           entt::dispatcher& dispatcher,
                           const MouseInfo& mouse);

  [[nodiscard]] virtual auto is_available(const DocumentModel& model) const -> bool = 0;

  [[nodiscard]] virtual auto get_type() const -> ToolType = 0;
};

}  // namespace tactile

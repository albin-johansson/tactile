// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "model/tool/tool.hpp"
#include "model/tool/tool_type.hpp"
#include "model/tool/tool_visitor.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

/// Manages the tools associated with a map document.
class ToolManager final : private Tool {
 public:
  TACTILE_DELETE_COPY(ToolManager);
  TACTILE_DECLARE_MOVE(ToolManager);

  ToolManager();

  ~ToolManager() noexcept override;

  void accept(ToolVisitor& visitor) const override;

  void select_tool(ToolType type, DocumentModel& model, entt::dispatcher& dispatcher);

  void set_stamp_random_mode(bool random);

  [[nodiscard]] auto is_enabled(ToolType type) const -> bool;

  [[nodiscard]] auto is_stamp_random() const -> bool;

  [[nodiscard]] auto is_available(const DocumentModel& model, ToolType type) const
      -> bool;

  void on_enabled(DocumentModel& model, entt::dispatcher& dispatcher) override;

  void on_disabled(DocumentModel& model, entt::dispatcher& dispatcher) override;

  void on_entered(DocumentModel& model, entt::dispatcher& dispatcher) override;

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

 private:
  struct Data;
  Unique<Data> mData;

  // Do not call these functions, they will just raise exceptions
  [[nodiscard, deprecated]] auto get_type() const -> ToolType override;
  [[nodiscard, deprecated]] auto is_available(const DocumentModel& model) const
      -> bool override;
};

}  // namespace tactile

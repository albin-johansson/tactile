// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "model/tool/tool.hpp"

namespace tactile {

class BucketTool final : public Tool {
 public:
  void accept(ToolVisitor& visitor) const override;

  void on_pressed(DocumentModel& model,
                  entt::dispatcher& dispatcher,
                  const MouseInfo& mouse) override;

  [[nodiscard]] auto is_available(const DocumentModel& model) const -> bool override;

  [[nodiscard]] auto get_type() const -> ToolType override { return ToolType::Bucket; }
};

}  // namespace tactile

// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "ui/widget/context_menu.hpp"

namespace tactile::ui {

class AddLayerContextMenu final : public ContextMenu {
 public:
  AddLayerContextMenu();

 protected:
  void on_update(const DocumentModel& model, entt::dispatcher& dispatcher) override;
};

}  // namespace tactile::ui

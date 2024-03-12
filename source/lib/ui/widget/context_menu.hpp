// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <entt/fwd.hpp>

#include "tactile/base/prelude.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, DocumentModel)

namespace tactile::ui {

// TODO deprecate
class ContextMenu {
 public:
  TACTILE_DEFAULT_COPY(ContextMenu);
  TACTILE_DEFAULT_MOVE(ContextMenu);

  explicit ContextMenu(const char* name);

  virtual ~ContextMenu() noexcept = default;

  void update(const DocumentModel& model, entt::dispatcher& dispatcher);

  void show();

 protected:
  virtual void on_update([[maybe_unused]] const DocumentModel& model,
                         [[maybe_unused]] entt::dispatcher& dispatcher)
  {
  }

 private:
  const char* mName {};
  bool mShow {};
};

}  // namespace tactile::ui

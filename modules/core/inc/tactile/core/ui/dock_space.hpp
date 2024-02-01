// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/ui/widget.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

/**
 * Manages the Dear ImGui dock space for the dock widgets.
 */
class TACTILE_CORE_API DockSpace final {
 public:
  /**
   * Updates the dock space.
   *
   * \note
   *    This function should be called once at the start of each frame, before
   *    any other widgets have been submitted.
   *
   * \param model
   * \param dispatcher
   */
  void update(const Model& model, EventDispatcher& dispatcher);

 private:
  Maybe<uint> mRootId;
  bool mDidInit {false};

  void _load_default_layout();
};

static_assert(WidgetType<DockSpace>);

}  // namespace tactile::core

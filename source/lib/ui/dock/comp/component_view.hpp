// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <entt/signal/fwd.hpp>

#include "tactile/base/container/string.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/util/uuid.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, Component)

namespace tactile::ui {

/// Shows a view of a single component, attached to a context.
void component_view(const UUID& context_id,
                    const Component& component,
                    const String& name,
                    entt::dispatcher& dispatcher);

}  // namespace tactile::ui

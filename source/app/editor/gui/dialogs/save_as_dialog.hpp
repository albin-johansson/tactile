#pragma once

#include <entt/entt.hpp>

namespace tactile {

/**
 * \brief Opens the blocking "Save As..." file dialog.
 *
 * \param dispatcher the event dispatcher that will be used.
 */
void show_save_as_dialog(entt::dispatcher& dispatcher);

}  // namespace tactile

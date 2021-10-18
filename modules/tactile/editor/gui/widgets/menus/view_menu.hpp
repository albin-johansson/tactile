#pragma once

#include <entt.hpp>  // dispatcher

namespace Tactile {

class Model;

/**
 * \brief Updates the "View" menu.
 *
 * \ingroup gui
 *
 * \param model the associated model.
 * \param dispatcher the event dispatcher that will be used.
 */
void UpdateViewMenu(const Model& model, entt::dispatcher& dispatcher);

}  // namespace Tactile

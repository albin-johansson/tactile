#pragma once

#include <entt/entt.hpp>  // dispatcher

namespace Tactile {

class Model;

/**
 * \brief Updates the state of all GUI components.
 *
 * \ingroup gui
 *
 * \param model the model that will be displayed.
 * \param dispatcher the event dispatcher that will be used.
 */
void UpdateGui(const Model& model, entt::dispatcher& dispatcher);

}  // namespace Tactile

#pragma once

#include <entt.hpp>  // dispatcher

namespace Tactile {

class Model;

/**
 * \brief Updates the state of the menu bar.
 *
 * \ingroup gui
 *
 * \param model the associated model.
 * \param dispatcher the event dispatcher that will be used.
 */
void UpdateMenuBar(const Model& model, entt::dispatcher& dispatcher);

}  // namespace Tactile

#pragma once

#include <entt.hpp>  // dispatcher

namespace Tactile {

class Model;

/**
 * \brief Updates the state of the layer dock widget.
 *
 * \ingroup gui
 *
 * \param model the associated model.
 * \param dispatcher the event dispatcher that will be used.
 */
void UpdateLayerDock(const Model& model, entt::dispatcher& dispatcher);

}  // namespace Tactile

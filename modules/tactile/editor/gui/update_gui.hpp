#pragma once

#include <entt/entt.hpp>  // dispatcher

namespace Tactile {

class Model;
class Icons;

/**
 * \brief Updates the state of all GUI components.
 *
 * \ingroup gui
 *
 * \param model the model that will be displayed.
 * \param icons handle to the loaded icons.
 * \param dispatcher the event dispatcher that will be used.
 */
void UpdateGui(const Model& model, const Icons& icons, entt::dispatcher& dispatcher);

}  // namespace Tactile

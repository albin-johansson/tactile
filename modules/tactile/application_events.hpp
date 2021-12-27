#pragma once

namespace Tactile {

class Application;

/**
 * \brief Subscribes all events to an application instance.
 *
 * \param app the application to connect the events to.
 */
void SubscribeToEvents(Application& app);

}  // namespace Tactile

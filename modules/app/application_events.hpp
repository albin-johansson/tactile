#pragma once

namespace tactile {

class application;

/**
 * \brief Subscribes all events to an application instance.
 *
 * \param app the application to connect the events to.
 */
void subscribe_to_events(application& app);

}  // namespace tactile

#pragma once

#include "app.hpp"

namespace tactile {

/**
 * @class app_connections
 *
 * @brief Used to initialize the connections for an `app` instance.
 *
 * @since 0.1.0
 *
 * @see `app`
 *
 * @headerfile app_connections.hpp
 */
class app_connections final {
 public:
  /**
   * @brief Initializes the connections for an `app` instance.
   *
   * @param app the app to initialize the connections for.
   *
   * @since 0.1.0
   */
  explicit app_connections(app& app);

 private:
  model::core_model* m_core;
  gui::window* m_window;

  void init_tilemap_connections(app& app) noexcept;

  void init_camera_connections(app& app) noexcept;

  void init_command_connections(app& app) noexcept;
};

}  // namespace tactile

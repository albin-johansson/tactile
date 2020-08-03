#pragma once

#include <QApplication>
#include <memory>
#include <utility>

#include "command_stack.hpp"
#include "core.hpp"
#include "tactile_fwd.hpp"
#include "tactile_types.hpp"
#include "window.hpp"

namespace tactile {

/**
 * @class app
 *
 * @brief Represents the tactile application.
 *
 * @see `QApplication`
 *
 * @since 0.1.0
 *
 * @headerfile app.hpp
 */
class app final : public QApplication {
 public:
  friend class app_connections;

  /**
   * @param argc the amount of command-line arguments.
   * @param argv the array of command-line arguments.
   *
   * @since 0.1.0
   */
  app(int argc, char** argv);

 private:
  std::unique_ptr<ui::window> m_window;
  model::core m_core;
  command_stack* m_commands{};  // TODO one stack per map

  [[nodiscard]] auto window_ptr() noexcept -> ui::window*
  {
    return m_window.get();
  }

  [[nodiscard]] auto core_ptr() noexcept -> model::core* { return &m_core; }

 private slots:
  void handle_undo();

  void handle_redo();

  void handle_add_row();

  void handle_add_col();

  void handle_remove_row();

  void handle_remove_col();

  void handle_resize_map();

  void handle_pan_up();

  void handle_pan_down();

  void handle_pan_right();

  void handle_pan_left();

  void handle_center_camera();

  void handle_new_tileset();
};

}  // namespace tactile

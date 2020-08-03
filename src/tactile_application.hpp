#pragma once

#include <QApplication>
#include <memory>

#include "tactile_core.hpp"
#include "tactile_fwd.hpp"
#include "tactile_window.hpp"

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
 * @headerfile tactile_application.hpp
 */
class app final : public QApplication {
 public:
  /**
   * @param argc the amount of command-line arguments.
   * @param argv the array of command-line arguments.
   *
   * @since 0.1.0
   */
  app(int argc, char** argv);

 private:
  std::unique_ptr<ui::window> m_window;
  std::unique_ptr<tactile_core> m_core;

  void init_connections() noexcept;

  /**
   * @brief Loads and applies the specified style sheet.
   *
   * @param styleSheet the file path of the QSS style-sheet.
   *
   * @since 0.1.0
   */
  void load_style_sheet(const char* styleSheet);

  /**
   * @brief A helper method for establishing a connection from the window to
   * the core.
   *
   * @tparam Sender the type of the sender functor.
   * @tparam Receiver the type of the receiver functor.
   *
   * @param s the function pointer to the signal.
   * @param r the function pointer/object that handles the signal.
   *
   * @since 0.1.0
   */
  template <typename Sender, typename Receiver>
  void window_to_core(Sender s, Receiver r)
  {
    connect(m_window.get(), s, m_core.get(), r);
  }

  /**
   * @brief A helper method for establishing a connection from the window to
   * this application instance.
   *
   * @tparam Sender the type of the sender functor.
   * @tparam Handler the type of the handler functor.
   *
   * @param s the function pointer to the signal.
   * @param r the function pointer/object that handles the signal.
   *
   * @since 0.1.0
   */
  template <typename Sender, typename Handler>
  void window_to_this(Sender s, Handler h)
  {
    connect(m_window.get(), s, this, h);
  }
};

}  // namespace tactile

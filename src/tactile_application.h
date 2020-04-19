#pragma once
#include <QApplication>

#include "smart_pointers.h"

class QPaintEvent;

namespace tactile {

class TactileWindow;
class TactileCore;

/**
 * The <code>TactileApplication</code> class is a subclass of
 * <code>QApplication</code> that represents the entire Tactile application.
 *
 * @see QApplication
 * @since 0.1.0
 */
class TactileApplication final : public QApplication {
 public:
  /**
   * @param argc the amount of command-line arguments.
   * @param argv the array of command-line arguments.
   * @since 0.1.0
   */
  TactileApplication(int argc, char** argv);

  ~TactileApplication() noexcept override;

 private:
  Unique<TactileWindow> m_window;
  Unique<TactileCore> m_core;

  void init_connections() noexcept;

  /**
   * Loads and applies the specified style sheet.
   *
   * @param styleSheet the file path of the QSS style-sheet.
   * @since 0.1.0
   */
  void load_style_sheet(const char* styleSheet);

  /**
   * A helper method for establishing a connection from the window to the core.
   *
   * @tparam Sender the type of the sender functor.
   * @tparam Receiver the type of the receiver functor.
   * @param s the function pointer to the signal.
   * @param r the function pointer/object that handles the signal.
   * @since 0.1.0
   */
  template <typename Sender, typename Receiver>
  void window_to_core(Sender s, Receiver r)
  {
    connect(m_window.get(), s, m_core.get(), r);
  }

  /**
   * A helper method for establishing a connection from the window to this
   * application instance.
   *
   * @tparam Sender the type of the sender functor.
   * @tparam Handler the type of the handler functor.
   * @param s the function pointer to the signal.
   * @param r the function pointer/object that handles the signal.
   * @since 0.1.0
   */
  template <typename Sender, typename Handler>
  void window_to_this(Sender s, Handler h)
  {
    connect(m_window.get(), s, this, h);
  }
};

}  // namespace tactile

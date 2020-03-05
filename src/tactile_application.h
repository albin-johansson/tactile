#pragma once
#include <QApplication>

#include "smart_pointers.h"

class QPaintEvent;

namespace tactile {

class TactileWindow;
class TactileEditor;

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

  /**
   * Loads and applies the specified style sheet.
   *
   * @param styleSheet the file path of the QSS style-sheet.
   * @since 0.1.0
   */
  void load_style_sheet(const char* styleSheet);  // TODO doc: does it throw?

 private:
  UniquePtr<TactileWindow> m_window;
  UniquePtr<TactileEditor> m_editor;

  void init_connections() noexcept;
};

}  // namespace tactile

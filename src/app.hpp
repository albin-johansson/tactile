#pragma once

#include <QApplication>
#include <QString>
#include <QUrl>
#include <memory>  // unique_ptr

#include "czstring.hpp"
#include "position.hpp"
#include "tileset.hpp"

namespace tactile {

namespace core {
class model;
}
namespace gui {
class window;
}

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
class app final : public QApplication
{
  Q_OBJECT

 public:
  /**
   * @param argc the amount of command-line arguments.
   * @param argv the array of command-line arguments.
   *
   * @since 0.1.0
   */
  app(int argc, char** argv);

  ~app() noexcept override;

 private:
  std::unique_ptr<gui::window> m_window;
  core::model* m_model{};

  void init_connections();

 private slots:
  void save_as(const QString& path);

  void open_map(const QString& path);

  void handle_resize_map();

  void handle_pan_up();

  void handle_pan_down();

  void handle_pan_right();

  void handle_pan_left();

  void handle_new_tileset();

  void tileset_selection_changed(const core::tileset::selection& selection);

  void handle_new_map();
};

}  // namespace tactile

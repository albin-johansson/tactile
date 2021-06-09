#pragma once

#include <QApplication>  // QApplication
#include <QString>       // QString
#include <QUrl>          // QUrl

#include "czstring.hpp"
#include "forward_declare.hpp"
#include "position.hpp"
#include "smart_pointers.hpp"
#include "tileset.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, model)
TACTILE_FORWARD_DECLARE(tactile::gui, Window)

namespace tactile {

/**
 * \class tactile_app
 *
 * \brief Represents the tactile application.
 *
 * \see `QApplication`
 *
 * \since 0.1.0
 *
 * \headerfile tactile_app.hpp
 */
class tactile_app final : public QApplication
{
  Q_OBJECT

 public:
  /**
   * \param argc the amount of command-line arguments.
   * \param argv the array of command-line arguments.
   *
   * \since 0.1.0
   */
  tactile_app(int argc, char** argv);

  ~tactile_app() noexcept override;

 private:
  unique<gui::Window> m_window;
  unique<core::model> m_model;

  void init_connections();

 private slots:
  void save();

  void save_as(const QString& path);

  void open_map(const QString& path);

  void handle_resize_map();

  void handle_pan_up();

  void handle_pan_down();

  void handle_pan_right();

  void handle_pan_left();

  void handle_new_tileset();

  void handle_new_map();
};

}  // namespace tactile

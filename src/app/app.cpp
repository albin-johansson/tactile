#include "app.hpp"

#include <qdockwidget.h>
#include <qpainter.h>

#include "app_connections.hpp"
#include "core_model.hpp"
#include "resize_dialog.hpp"
#include "setup_app.hpp"
#include "tileset_dialog.hpp"
#include "window.hpp"

namespace tactile {

using model::core_model;

app::app(int argc, char** argv)
    : QApplication{argc, argv}, m_core{new core_model{}}
{
  setup_app();

  m_window = std::make_unique<gui::window>();

  app_connections{*this};

  // do this to indicate what tool is active
  //  QPixmap pixmap{":/resources/icons/icons8/color/64/eraser.png"};
  //  QCursor c{pixmap};
  //  QApplication::setOverrideCursor(c);

  m_window->show();
}

app::~app() noexcept
{
  delete m_core;
}

void app::handle_resize_map()
{
  if (m_core->has_active_map()) {
    gui::resize_dialog::spawn(
        [this](int rows, int cols) { m_core->resize_map(rows, cols); });
  }
}

void app::handle_pan_up()
{
  if (const auto tileSize = m_core->tile_size(); tileSize) {
    m_window->handle_move_camera(0, *tileSize);
  }
}

void app::handle_pan_down()
{
  if (const auto tileSize = m_core->tile_size(); tileSize) {
    m_window->handle_move_camera(0, -(*tileSize));
  }
}

void app::handle_pan_right()
{
  if (const auto tileSize = m_core->tile_size(); tileSize) {
    m_window->handle_move_camera(-(*tileSize), 0);
  }
}

void app::handle_pan_left()
{
  if (const auto tileSize = m_core->tile_size(); tileSize) {
    m_window->handle_move_camera(*tileSize, 0);
  }
}

void app::handle_new_tileset()
{
  gui::tileset_dialog::spawn([this](const QImage& image,
                                    int tileWidth,
                                    int tileHeight,
                                    const QString& name) {
    if (const auto id = m_core->add_tileset(image, tileWidth, tileHeight); id) {
      m_window->handle_add_tileset(image, *id, tileWidth, tileHeight, name);
    }
  });
}

void app::handle_new_map()
{
  const auto id = m_core->add_map();
  m_window->handle_new_map(m_core->get_map(id), id);
}

}  // namespace tactile

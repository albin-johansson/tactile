#include "app.hpp"

#include <QDockWidget>
#include <QPainter>

#include "add_col.hpp"
#include "add_row.hpp"
#include "app_connections.hpp"
#include "core.hpp"
#include "remove_col.hpp"
#include "remove_row.hpp"
#include "resize_dialog.hpp"
#include "resize_map.hpp"
#include "set_style_sheet.hpp"
#include "setup_app.hpp"
#include "tileset_dialog.hpp"
#include "tileset_info.hpp"
#include "window.hpp"

using namespace tactile::ui;
using namespace tactile::model;

namespace tactile {

app::app(int argc, char** argv) : QApplication{argc, argv}
{
  setup_app();

  m_window = std::make_unique<window>();
  m_commands = new command_stack{this};

  app_connections{*this};
  set_style_sheet(*this, ":/resources/tactile_light.qss");

  // do this to indicate what tool is active
  //  QPixmap pixmap{":/resources/icons/icons8/color/64/eraser.png"};
  //  QCursor c{pixmap};
  //  QApplication::setOverrideCursor(c);

  m_window->show();
}

void app::handle_undo()
{
  m_commands->undo();
}

void app::handle_redo()
{
  m_commands->redo();
}

void app::handle_add_row()
{
  if (m_core.has_active_map()) {
    m_commands->push<cmd::add_row>(&m_core);
  }
}

void app::handle_add_col()
{
  if (m_core.has_active_map()) {
    m_commands->push<cmd::add_col>(&m_core);
  }
}

void app::handle_remove_row()
{
  if (m_core.has_active_map()) {
    m_commands->push<cmd::remove_row>(&m_core);
  }
}

void app::handle_remove_col()
{
  if (m_core.has_active_map()) {
    m_commands->push<cmd::remove_col>(&m_core);
  }
}

void app::handle_resize_map()
{
  if (m_core.has_active_map()) {
    resize_dialog dialog;

    if (dialog.exec()) {
      const auto rows = *dialog.chosen_height();
      const auto cols = *dialog.chosen_width();
      m_commands->push<cmd::resize_map>(&m_core, rows, cols);
    }
  }
}

void app::handle_pan_up()
{
  if (const auto tileSize = m_core.tile_size(); tileSize) {
    m_window->handle_move_camera(0, *tileSize);
  }
}

void app::handle_pan_down()
{
  if (const auto tileSize = m_core.tile_size(); tileSize) {
    m_window->handle_move_camera(0, -(*tileSize));
  }
}

void app::handle_pan_right()
{
  if (const auto tileSize = m_core.tile_size(); tileSize) {
    m_window->handle_move_camera(-(*tileSize), 0);
  }
}

void app::handle_pan_left()
{
  if (const auto tileSize = m_core.tile_size(); tileSize) {
    m_window->handle_move_camera(*tileSize, 0);
  }
}

void app::handle_center_camera()
{
  const auto width = m_core.map_width();
  const auto height = m_core.map_height();
  if (width && height) {
    m_window->handle_center_camera(*width, *height);
  }
}

void app::handle_new_tileset()
{
  tileset_dialog dialog;
  if (dialog.exec()) {
    const auto& image = dialog.chosen_image();
    const auto tileWidth = dialog.chosen_width();
    const auto tileHeight = dialog.chosen_height();
    const auto imageName = dialog.image_name();

    if (!image.isNull() && tileWidth && tileHeight) {
      const auto id = m_core.add_tileset(image, *tileWidth, *tileHeight);
      if (id) {
        tileset_info info{image, *id, *tileWidth, *tileHeight};
        m_window->handle_add_tileset(info, imageName ? *imageName : "Untitled");
      }
    }
  }
}

}  // namespace tactile

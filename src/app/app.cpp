#include "app.hpp"

#include "app_connections.hpp"
#include "model.hpp"
#include "resize_dialog.hpp"
#include "save_service.hpp"
#include "service/open/open_map.hpp"
#include "setup_app.hpp"
#include "tileset_dialog.hpp"
#include "window.hpp"

namespace tactile {

using core::col_t;
using core::row_t;

app::app(int argc, char** argv)
    : QApplication{argc, argv},
      m_model{new core::model{}}
{
  setup_app();

  m_window = std::make_unique<gui::window>();

  app_connections{this};

  m_window->show();
}

app::~app() noexcept
{
  delete m_model;
}

void app::save_as(const QString& path)
{
  if (const auto* document = m_model->current_document()) {
    service::save(path, *document);
  }
}

void app::open_map(const QString& path)
{
  auto* document = service::open_map(path);

  const auto mapId = m_model->add_map(document);
  m_window->handle_new_map(document, mapId);

  document->each_tileset(
      [&](tileset_id tilesetId, const core::tileset& tileset) {
        m_window->handle_add_tileset(mapId, tilesetId, tileset);
      });
}

void app::handle_resize_map()
{
  if (m_model->has_active_map()) {
    gui::resize_dialog::spawn(
        [this](row_t rows, col_t cols) { m_model->resize_map(rows, cols); });
  }
}

void app::handle_pan_up()
{
  if (const auto* document = m_model->current_document()) {
    m_window->handle_move_camera(0, document->current_tile_size());
  }
}

void app::handle_pan_down()
{
  if (const auto* document = m_model->current_document()) {
    m_window->handle_move_camera(0, -document->current_tile_size());
  }
}

void app::handle_pan_right()
{
  if (const auto* document = m_model->current_document()) {
    m_window->handle_move_camera(-document->current_tile_size(), 0);
  }
}

void app::handle_pan_left()
{
  if (const auto* document = m_model->current_document()) {
    m_window->handle_move_camera(document->current_tile_size(), 0);
  }
}

void app::handle_new_tileset()
{
  gui::tileset_dialog::spawn([this](const QImage& image,
                                    const QString& path,
                                    tile_width tileWidth,
                                    tile_height tileHeight,
                                    const QString& name) {
    m_model->ui_added_tileset(image, path, name, tileWidth, tileHeight);
  });
}

void app::tileset_selection_changed(core::position topLeft,
                                    core::position bottomRight)
{
  m_model->update_tileset_selection(topLeft, bottomRight);
}

void app::handle_new_map()
{
  const auto id = m_model->add_map();
  m_window->handle_new_map(m_model->get_document(id), id);
}

}  // namespace tactile

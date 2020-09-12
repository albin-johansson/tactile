#include "model.hpp"

#include "tileset.hpp"

namespace tactile::core {

model::model()
    : m_maps{std::make_unique<map_manager>()},
      m_tilesets{std::make_unique<tileset_manager>()},
      m_tools{this}
{
  connect(m_maps.get(), &map_manager::switched_map, this, &model::switched_map);

  connect(m_maps.get(),
          &map_manager::undo_state_updated,
          this,
          &model::undo_state_updated);

  connect(m_maps.get(),
          &map_manager::redo_state_updated,
          this,
          &model::redo_state_updated);

  connect(m_maps.get(),
          &map_manager::undo_text_updated,
          this,
          &model::undo_text_updated);

  connect(m_maps.get(),
          &map_manager::redo_text_updated,
          this,
          &model::redo_text_updated);
}

void model::undo()
{
  if (auto* document = current_map_document()) {
    document->undo();
    emit redraw();
  }
}

void model::redo()
{
  if (auto* document = current_map_document()) {
    document->redo();
    emit redraw();
  }
}

void model::resize_map(int nRows, int nCols)
{
  if (auto* document = current_map_document()) {
    document->resize(nRows, nCols);
    emit redraw();
  }
}

void model::add_row()
{
  if (auto* document = current_map_document()) {
    document->add_row();
    emit redraw();
  }
}

void model::add_col()
{
  if (auto* document = current_map_document()) {
    document->add_column();
    emit redraw();
  }
}

void model::remove_row()
{
  if (auto* document = current_map_document()) {
    document->remove_row();
    emit redraw();
  }
}

void model::remove_col()
{
  if (auto* document = current_map_document()) {
    document->remove_column();
    emit redraw();
  }
}

void model::select_layer(layer_id id)
{
  if (auto* map = current_map()) {
    map->select_layer(id);
    emit redraw();
  }
}

void model::increase_tile_size()
{
  if (auto* map = current_map()) {
    map->get_tile_size().increase();
    emit redraw();
  }
}

void model::decrease_tile_size()
{
  if (auto* map = current_map()) {
    map->get_tile_size().decrease();
    emit redraw();
  }
}

void model::reset_tile_size()
{
  if (auto* map = current_map()) {
    map->get_tile_size().reset();
    emit redraw();
  }
}

void model::select_tool(tool_id tool)
{
  m_tools.select(tool);
}

void model::select_tileset(tileset_id id)
{
  m_tilesets->select(id);
}

auto model::add_map() -> map_id
{
  return m_maps->add();
}

void model::close_map(map_id id)
{
  m_maps->close(id);
}

void model::select_map(map_id id)
{
  m_maps->select(id);
}

auto model::add_tileset(const QImage& image,
                        tile_width tileWidth,
                        tile_height tileHeight) -> std::optional<tileset_id>
{
  if (!image.isNull()) {
    return m_tilesets->emplace(image, tileWidth, tileHeight);
  } else {
    return std::nullopt;
  }
}

void model::remove_tileset(tileset_id id)
{
  const auto [first, last] = m_tilesets->range_of(id);

  for (auto& [mapID, document] : *m_maps) {
    auto* map = document->get();

    for (auto i = first; i < last; ++i) {
      map->remove_all(i);
    }
  }

  m_tilesets->remove(id);
  emit redraw();
}

void model::update_tileset_selection(position topLeft, position bottomRight)
{
  m_tilesets->set_selection(topLeft, bottomRight);
}

auto model::get_map(map_id id) -> map*
{
  return m_maps->at(id);
}

auto model::has_active_map() const noexcept -> bool
{
  return m_maps->has_active_map();
}

void model::mouse_pressed(QMouseEvent* event, QPointF mapPosition)
{
  m_tools.pressed(event, mapPosition);
}

void model::mouse_moved(QMouseEvent* event, QPointF mapPosition)
{
  m_tools.moved(event, mapPosition);
}

void model::mouse_released(QMouseEvent* event, QPointF mapPosition)
{
  m_tools.released(event, mapPosition);
}

void model::mouse_entered(QEvent* event)
{
  m_tools.entered(event);
}

void model::mouse_exited(QEvent* event)
{
  m_tools.exited(event);
}

auto model::current_map_document() -> map_document*
{
  return m_maps->current_document();
}

auto model::current_map_document() const -> const map_document*
{
  return m_maps->current_document();
}

auto model::current_map() -> map*
{
  return m_maps->current_map();
}

auto model::current_map() const -> const map*
{
  return m_maps->current_map();
}

auto model::current_tileset() const -> const tileset*
{
  return m_tilesets->current_tileset();
}

}  // namespace tactile::core

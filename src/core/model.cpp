#include "model.hpp"

#include "tileset.hpp"

namespace tactile::core {

model::model() : m_tools{this}, m_tilesets{std::make_unique<tileset_model>()}
{}

void model::send_undo_redo_update()
{
  const auto* mapModel = current_map_model();

  emit undo_state_updated(mapModel && mapModel->can_undo());
  emit redo_state_updated(mapModel && mapModel->can_redo());
  emit undo_text_updated(mapModel ? mapModel->undo_text()
                                  : QStringLiteral(u""));
  emit redo_text_updated(mapModel ? mapModel->redo_text()
                                  : QStringLiteral(u""));
}

void model::undo()
{
  if (auto* map = current_map_model()) {
    map->undo();
    emit redraw();
  }
}

void model::redo()
{
  if (auto* map = current_map_model()) {
    map->redo();
    emit redraw();
  }
}

void model::resize_map(int nRows, int nCols)
{
  if (auto* map = current_map_model()) {
    map->resize(nRows, nCols);
    emit redraw();
  }
}

void model::add_row()
{
  if (auto* map = current_map_model()) {
    map->add_row();
    emit redraw();
  }
}

void model::add_col()
{
  if (auto* map = current_map_model()) {
    map->add_column();
    emit redraw();
  }
}

void model::remove_row()
{
  if (auto* map = current_map_model()) {
    map->remove_row();
    emit redraw();
  }
}

void model::remove_col()
{
  if (auto* map = current_map_model()) {
    map->remove_column();
    emit redraw();
  }
}

void model::select_tool(tool_id tool)
{
  m_tools.select(tool);
}

auto model::add_map() -> map_id
{
  const auto id = m_nextMapID;
  Q_ASSERT(!m_maps.count(id));

  auto* map = new map_model{this};

  auto bind = [map, this](auto&& signal, auto&& slot) {
    connect(map, signal, this, slot);
  };

  bind(&map_model::undo_state_updated, &model::undo_state_updated);
  bind(&map_model::redo_state_updated, &model::redo_state_updated);
  bind(&map_model::undo_text_updated, &model::undo_text_updated);
  bind(&map_model::redo_text_updated, &model::redo_text_updated);

  m_maps.emplace(id, map);
  m_currentMapID = id;

  ++m_nextMapID;

  send_undo_redo_update();

  return id;
}

void model::handle_close_map(map_id id)
{
  Q_ASSERT(m_maps.contains(id));

  m_maps.at(id)->disconnect();
  m_maps.erase(id);

  if (m_currentMapID && (m_currentMapID->get() == id.get())) {
    m_currentMapID = std::nullopt;
    send_undo_redo_update();
  }

  if (!m_maps.empty()) {
    const auto begin = m_maps.begin();
    m_currentMapID = begin->first;
    send_undo_redo_update();
  }
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

  for (auto& [mapID, mapModel] : m_maps) {
    auto* map = mapModel->get();

    for (auto i = first; i < last; ++i) {
      map->remove_all(i);
    }
  }
  m_tilesets->remove(id);
  emit redraw();
}

void model::select_tileset(tileset_id id)
{
  m_tilesets->select(id);
}

void model::update_tileset_selection(position topLeft, position bottomRight)
{
  m_tilesets->set_selection(topLeft, bottomRight);
}

auto model::rows() const -> std::optional<int>
{
  if (auto* map = current_map_model()) {
    return map->rows();
  } else {
    return std::nullopt;
  }
}

auto model::cols() const -> std::optional<int>
{
  if (auto* map = current_map_model()) {
    return map->columns();
  } else {
    return std::nullopt;
  }
}

auto model::map_width() const -> std::optional<int>
{
  if (auto* map = current_map_model()) {
    return map->width();
  } else {
    return std::nullopt;
  }
}

auto model::map_height() const -> std::optional<int>
{
  if (auto* map = current_map_model()) {
    return map->height();
  } else {
    return std::nullopt;
  }
}

auto model::tile_size() const -> std::optional<int>
{
  if (auto* map = current_map_model()) {
    return map->current_tile_size();
  } else {
    return std::nullopt;
  }
}

auto model::get_map(map_id id) -> map*
{
  if (const auto it = m_maps.find(id); it != m_maps.end()) {
    return it->second->get();
  } else {
    return nullptr;
  }
}

void model::select_layer(layer_id id)
{
  if (auto* map = current_map_model()) {
    map->select_layer(id);
    emit redraw();
  }
}

void model::select_map(map_id id)
{
  Q_ASSERT(m_maps.count(id));

  if (m_currentMapID && (m_currentMapID->get() != id.get())) {
    m_currentMapID = id;

    send_undo_redo_update();

    emit switched_map(id);
  }
}

auto model::has_active_map() const noexcept -> bool
{
  return m_currentMapID.has_value();
}

void model::handle_increase_tile_size()
{
  if (auto* map = current_map_model()) {
    map->increase_tile_size();
    emit redraw();
  }
}

void model::handle_decrease_tile_size()
{
  if (auto* map = current_map_model()) {
    map->decrease_tile_size();
    emit redraw();
  }
}

void model::handle_reset_tile_size()
{
  if (auto* map = current_map_model()) {
    map->reset_tile_size();
    emit redraw();
  }
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

auto model::current_map_model() -> map_model*
{
  return m_currentMapID ? m_maps.at(m_currentMapID.value()) : nullptr;
}

auto model::current_map_model() const -> const map_model*
{
  return m_currentMapID ? m_maps.at(m_currentMapID.value()) : nullptr;
}

auto model::current_raw_map() -> map*
{
  auto* model = current_map_model();
  return model ? model->get() : nullptr;
}

auto model::current_raw_map() const -> const map*
{
  const auto* model = current_map_model();
  return model ? model->get() : nullptr;
}

auto model::current_tileset() const -> const tileset*
{
  return m_tilesets->current_tileset();
}

auto model::current_tileset_id() const -> std::optional<tileset_id>
{
  return m_tilesets->current_tileset_id();
}

}  // namespace tactile::core

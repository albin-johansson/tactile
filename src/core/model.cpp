#include "model.hpp"

#include "tileset.hpp"

namespace tactile::core {

model::model() : m_tools{this}, m_tilesets{std::make_unique<tileset_model>()}
{}

void model::undo()
{
  if (auto* map = current_map()) {
    map->undo();
    emit redraw_requested();
  }
}

void model::redo()
{
  if (auto* map = current_map()) {
    map->redo();
    emit redraw_requested();
  }
}

void model::resize_map(int nRows, int nCols)
{
  if (auto* map = current_map()) {
    map->resize(nRows, nCols);
    emit redraw_requested();
  }
}

void model::add_row()
{
  if (auto* map = current_map()) {
    map->add_row();
    emit redraw_requested();
  }
}

void model::add_col()
{
  if (auto* map = current_map()) {
    map->add_column();
    emit redraw_requested();
  }
}

void model::remove_row()
{
  if (auto* map = current_map()) {
    map->remove_row();
    emit redraw_requested();
  }
}

void model::remove_col()
{
  if (auto* map = current_map()) {
    map->remove_column();
    emit redraw_requested();
  }
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

  return id;
}

void model::handle_close_map(map_id id)
{
  Q_ASSERT(m_maps.count(id));

  auto* map = m_maps.at(id);
  map->disconnect();

  m_maps.erase(id);

  if (m_currentMapID && (m_currentMapID->get() == id.get())) {
    m_currentMapID = std::nullopt;
  }

  if (!m_maps.empty()) {
    const auto begin = m_maps.begin();
    m_currentMapID = begin->first;
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
  emit redraw_requested();
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
  if (auto* map = current_map()) {
    return map->rows();
  } else {
    return std::nullopt;
  }
}

auto model::cols() const -> std::optional<int>
{
  if (auto* map = current_map()) {
    return map->columns();
  } else {
    return std::nullopt;
  }
}

auto model::map_width() const -> std::optional<int>
{
  if (auto* map = current_map()) {
    return map->width();
  } else {
    return std::nullopt;
  }
}

auto model::map_height() const -> std::optional<int>
{
  if (auto* map = current_map()) {
    return map->height();
  } else {
    return std::nullopt;
  }
}

auto model::tile_size() const -> std::optional<int>
{
  if (auto* map = current_map()) {
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
  if (auto* map = current_map()) {
    map->select_layer(id);
    emit redraw_requested();
  }
}

void model::select_map(map_id id)
{
  Q_ASSERT(m_maps.count(id));

  if (m_currentMapID && (m_currentMapID->get() != id.get())) {
    m_currentMapID = id;

    auto* map = current_map();
    Q_ASSERT(map);

    emit undo_state_updated(map->can_undo());
    emit redo_state_updated(map->can_redo());
    emit undo_text_updated(map->undo_text());
    emit redo_text_updated(map->redo_text());

    emit switched_map(id);

    // TODO need to signal window to switch active tab
  }
}

void model::set_tile(const position& pos, tile_id id)
{
  if (auto* map = current_map()) {
    emit redraw_requested();
  }
}

auto model::has_active_map() const noexcept -> bool
{
  return m_currentMapID.has_value();
}

void model::handle_increase_tile_size()
{
  if (auto* map = current_map()) {
    map->increase_tile_size();
    emit redraw_requested();
  }
}

void model::handle_decrease_tile_size()
{
  if (auto* map = current_map()) {
    map->decrease_tile_size();
    emit redraw_requested();
  }
}

void model::handle_reset_tile_size()
{
  if (auto* map = current_map()) {
    map->reset_tile_size();
    emit redraw_requested();
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

auto model::current_map() -> map_model*
{
  return m_currentMapID ? m_maps.at(m_currentMapID.value()) : nullptr;
}

auto model::current_map() const -> const map_model*
{
  return m_currentMapID ? m_maps.at(m_currentMapID.value()) : nullptr;
}

auto model::current_tileset() const -> const tileset*
{
  return m_tilesets->current_tileset();
}

}  // namespace tactile::core

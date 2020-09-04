#include "core_model.hpp"

#include "tileset.hpp"

namespace tactile::model {

void core_model::undo()
{
  if (auto* map = current_map()) {
    map->undo();
    emit redraw_requested();
  }
}

void core_model::redo()
{
  if (auto* map = current_map()) {
    map->redo();
    emit redraw_requested();
  }
}

void core_model::resize_map(int nRows, int nCols)
{
  if (auto* map = current_map()) {
    map->resize(nRows, nCols);
    emit redraw_requested();
  }
}

void core_model::add_row()
{
  if (auto* map = current_map()) {
    map->add_row();
    emit redraw_requested();
  }
}

void core_model::add_col()
{
  if (auto* map = current_map()) {
    map->add_column();
    emit redraw_requested();
  }
}

void core_model::remove_row()
{
  if (auto* map = current_map()) {
    map->remove_row();
    emit redraw_requested();
  }
}

void core_model::remove_col()
{
  if (auto* map = current_map()) {
    map->remove_column();
    emit redraw_requested();
  }
}

auto core_model::add_map() -> map_id
{
  const auto id = m_nextMapID;
  Q_ASSERT(!m_maps.count(id));

  auto* map = new map_model{this};

  auto bind = [map, this](auto&& signal, auto&& slot) {
    connect(map, signal, this, slot);
  };

  bind(&map_model::undo_state_updated, &core_model::undo_state_updated);
  bind(&map_model::redo_state_updated, &core_model::redo_state_updated);
  bind(&map_model::undo_text_updated, &core_model::undo_text_updated);
  bind(&map_model::redo_text_updated, &core_model::redo_text_updated);

  m_maps.emplace(id, map);
  m_currentMapID = id;

  ++m_nextMapID;

  return id;
}

void core_model::handle_close_map(map_id id)
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

auto core_model::add_tileset(const QImage& image,
                             tile_width tileWidth,
                             tile_height tileHeight)
    -> std::optional<tileset_id>
{
  if (!image.isNull()) {
    return m_tilesets.emplace(image, tileWidth, tileHeight);
  } else {
    return std::nullopt;
  }
}

auto core_model::rows() const -> std::optional<int>
{
  if (auto* map = current_map()) {
    return map->rows();
  } else {
    return std::nullopt;
  }
}

auto core_model::cols() const -> std::optional<int>
{
  if (auto* map = current_map()) {
    return map->columns();
  } else {
    return std::nullopt;
  }
}

auto core_model::map_width() const -> std::optional<int>
{
  if (auto* map = current_map()) {
    return map->width();
  } else {
    return std::nullopt;
  }
}

auto core_model::map_height() const -> std::optional<int>
{
  if (auto* map = current_map()) {
    return map->height();
  } else {
    return std::nullopt;
  }
}

auto core_model::tile_size() const -> std::optional<int>
{
  if (auto* map = current_map()) {
    return map->current_tile_size();
  } else {
    return std::nullopt;
  }
}

auto core_model::get_map(map_id id) -> tilemap*
{
  if (const auto it = m_maps.find(id); it != m_maps.end()) {
    return it->second->get();
  } else {
    return nullptr;
  }
}

void core_model::select_layer(layer_id id)
{
  if (auto* map = current_map()) {
    map->select_layer(id);
    emit redraw_requested();
  }
}

void core_model::select_map(map_id id)
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

auto core_model::has_active_map() const noexcept -> bool
{
  return m_currentMapID.has_value();
}

void core_model::handle_increase_tile_size()
{
  if (auto* map = current_map()) {
    map->increase_tile_size();
    emit redraw_requested();
  }
}

void core_model::handle_decrease_tile_size()
{
  if (auto* map = current_map()) {
    map->decrease_tile_size();
    emit redraw_requested();
  }
}

void core_model::handle_reset_tile_size()
{
  if (auto* map = current_map()) {
    map->reset_tile_size();
    emit redraw_requested();
  }
}

auto core_model::current_map() -> map_model*
{
  return m_currentMapID ? m_maps.at(m_currentMapID.value()) : nullptr;
}

auto core_model::current_map() const -> const map_model*
{
  return m_currentMapID ? m_maps.at(m_currentMapID.value()) : nullptr;
}

}  // namespace tactile::model

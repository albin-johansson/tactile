#include "model.hpp"

#include "tileset.hpp"

namespace tactile::core {

model::model() : m_tools{this}, m_tilesets{std::make_unique<tileset_model>()}
{}

void model::send_undo_redo_update()
{
  const auto* document = current_map_document();

  emit undo_state_updated(document && document->can_undo());
  emit redo_state_updated(document && document->can_redo());
  emit undo_text_updated(document ? document->undo_text()
                                  : QStringLiteral(u""));
  emit redo_text_updated(document ? document->redo_text()
                                  : QStringLiteral(u""));
}

void model::undo()
{
  if (auto* map = current_map_document()) {
    map->undo();
    emit redraw();
  }
}

void model::redo()
{
  if (auto* map = current_map_document()) {
    map->redo();
    emit redraw();
  }
}

void model::resize_map(int nRows, int nCols)
{
  if (auto* map = current_map_document()) {
    map->resize(nRows, nCols);
    emit redraw();
  }
}

void model::add_row()
{
  if (auto* map = current_map_document()) {
    map->add_row();
    emit redraw();
  }
}

void model::add_col()
{
  if (auto* map = current_map_document()) {
    map->add_column();
    emit redraw();
  }
}

void model::remove_row()
{
  if (auto* map = current_map_document()) {
    map->remove_row();
    emit redraw();
  }
}

void model::remove_col()
{
  if (auto* map = current_map_document()) {
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
  Q_ASSERT(!m_mapDocuments.count(id));

  auto* map = new map_document{this};

  auto bind = [map, this](auto&& signal, auto&& slot) {
    connect(map, signal, this, slot);
  };

  bind(&map_document::undo_state_updated, &model::undo_state_updated);
  bind(&map_document::redo_state_updated, &model::redo_state_updated);
  bind(&map_document::undo_text_updated, &model::undo_text_updated);
  bind(&map_document::redo_text_updated, &model::redo_text_updated);

  m_mapDocuments.emplace(id, map);
  m_currentMapID = id;

  ++m_nextMapID;

  send_undo_redo_update();

  return id;
}

void model::handle_close_map(map_id id)
{
  Q_ASSERT(m_mapDocuments.contains(id));

  m_mapDocuments.at(id)->disconnect();
  m_mapDocuments.erase(id);

  if (m_currentMapID && (m_currentMapID->get() == id.get())) {
    m_currentMapID = std::nullopt;
    send_undo_redo_update();
  }

  if (!m_mapDocuments.empty()) {
    const auto begin = m_mapDocuments.begin();
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

  for (auto& [mapID, document] : m_mapDocuments) {
    auto* map = document->get();

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
  if (auto* map = current_map_document()) {
    return map->get()->rows();
  } else {
    return std::nullopt;
  }
}

auto model::cols() const -> std::optional<int>
{
  if (auto* map = current_map_document()) {
    return map->get()->cols();
  } else {
    return std::nullopt;
  }
}

auto model::map_width() const -> std::optional<int>
{
  if (auto* document = current_map_document()) {
    return document->get()->width();
  } else {
    return std::nullopt;
  }
}

auto model::map_height() const -> std::optional<int>
{
  if (auto* document = current_map_document()) {
    return document->get()->height();
  } else {
    return std::nullopt;
  }
}

auto model::tile_size() const -> std::optional<int>
{
  if (auto* document = current_map_document()) {
    return document->get()->get_tile_size().get();
  } else {
    return std::nullopt;
  }
}

auto model::get_map(map_id id) -> map*
{
  if (const auto it = m_mapDocuments.find(id); it != m_mapDocuments.end()) {
    return it->second->get();
  } else {
    return nullptr;
  }
}

void model::select_layer(layer_id id)
{
  if (auto* document = current_map_document()) {
    document->get()->select_layer(id);
    emit redraw();
  }
}

void model::select_map(map_id id)
{
  Q_ASSERT(m_mapDocuments.count(id));

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
  if (auto* document = current_map_document()) {
    document->get()->get_tile_size().increase();
    emit redraw();
  }
}

void model::handle_decrease_tile_size()
{
  if (auto* document = current_map_document()) {
    document->get()->get_tile_size().decrease();
    emit redraw();
  }
}

void model::handle_reset_tile_size()
{
  if (auto* document = current_map_document()) {
    document->get()->get_tile_size().reset();
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

auto model::current_map_document() -> map_document*
{
  return m_currentMapID ? m_mapDocuments.at(m_currentMapID.value()) : nullptr;
}

auto model::current_map_document() const -> const map_document*
{
  return m_currentMapID ? m_mapDocuments.at(m_currentMapID.value()) : nullptr;
}

auto model::current_raw_map() -> map*
{
  auto* model = current_map_document();
  return model ? model->get() : nullptr;
}

auto model::current_raw_map() const -> const map*
{
  const auto* model = current_map_document();
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

#include "core.hpp"

#include <QImage>

#include "tilemap.hpp"
#include "tileset.hpp"
#include "tileset_manager.hpp"

namespace tactile::model {

core::core() : m_sheetManager{tileset_manager::unique()}
{}

auto core::unique() -> std::unique_ptr<core>
{
  return std::make_unique<core>();
}

void core::handle_new_map(int id) noexcept
{
  if (m_maps.count(id)) {
    qWarning("Editor core already had tilemap associated with %i!", id);
  } else {
    m_maps.insert({id, tilemap::unique(5, 5)});
    m_activeMapIndex = id;
  }
}

void core::handle_close_map(int id) noexcept
{
  if (m_maps.count(id)) {
    m_maps.erase(id);

    if (m_activeMapIndex == id) {
      m_activeMapIndex = std::nullopt;
    }

    if (!m_maps.empty()) {
      const auto begin = m_maps.begin();
      m_activeMapIndex = begin->first;
    }
  } else {
    qWarning("Attempted to remove tilemap that doesn't exist: %i!", id);
  }
}

void core::open_map(czstring)
{
  // TODO parse Tactile or TMX formats
  qWarning("\"Open map\" isn't implemented!");
}

void core::save_as(czstring) const
{
  qWarning("\"Save as\" isn't implemented!");
}

auto core::add_tileset(const std::shared_ptr<QImage>& image,
                       int tileWidth,
                       int tileHeight) noexcept -> std::optional<int>
{
  if (!image->isNull()) {
    return m_sheetManager->add(tileset::unique(image, tileWidth, tileHeight));
  } else {
    return std::nullopt;
  }
}

void core::set_rows(int nRows) noexcept
{
  if (auto* map = active_map(); map) {
    map->set_rows(nRows);
  }
}

void core::set_cols(int nCols) noexcept
{
  if (auto* map = active_map(); map) {
    map->set_cols(nCols);
  }
}

auto core::rows() const noexcept -> std::optional<int>
{
  return maybe_get<int>([](const tilemap& map) noexcept { return map.rows(); });
}

auto core::cols() const noexcept -> std::optional<int>
{
  return maybe_get<int>([](const tilemap& map) noexcept { return map.cols(); });
}

auto core::map_width() const noexcept -> std::optional<int>
{
  return maybe_get<int>(
      [](const tilemap& map) noexcept { return map.width(); });
}

auto core::map_height() const noexcept -> std::optional<int>
{
  return maybe_get<int>(
      [](const tilemap& map) noexcept { return map.height(); });
}

auto core::tile_size() const noexcept -> std::optional<int>
{
  return maybe_get<int>(
      [](const tilemap& map) noexcept { return map.get_tile_size().get(); });
}

void core::select_layer(int index) noexcept
{
  if (auto* map = active_map(); map) {
    map->select(index);
    emit request_update();
  }
}

void core::select_map(int id) noexcept
{
  if (m_maps.count(id)) {
    m_activeMapIndex = id;
  }
}

auto core::has_active_map() const noexcept -> bool
{
  return m_activeMapIndex.has_value();
}

void core::handle_increase_tile_size() noexcept
{
  if (auto* map = active_map(); map) {
    map->get_tile_size().increase();
    emit request_update();
  }
}

void core::handle_decrease_tile_size() noexcept
{
  if (auto* map = active_map(); map) {
    map->get_tile_size().decrease();
    emit request_update();
  }
}

void core::handle_reset_tile_size() noexcept
{
  if (auto* map = active_map(); map) {
    map->get_tile_size().reset();
    emit request_update();
  }
}

void core::handle_draw(QPainter& painter) const noexcept
{
  if (auto* map = active_map(); map) {
    map->draw(painter);
  }
}

void core::handle_add_row() noexcept
{
  if (auto* map = active_map(); map) {
    map->add_row();
    emit request_update();
  }
}

void core::handle_add_col() noexcept
{
  if (auto* map = active_map(); map) {
    map->add_col();
    emit request_update();
  }
}

void core::handle_remove_row() noexcept
{
  if (auto* map = active_map(); map) {
    map->remove_row();
    emit request_update();
  }
}

void core::handle_remove_col() noexcept
{
  if (auto* map = active_map(); map) {
    map->remove_col();
    emit request_update();
  }
}

auto core::active_map() noexcept -> tilemap*
{
  if (m_activeMapIndex) {
    return m_maps.at(*m_activeMapIndex).get();
  } else {
    return nullptr;
  }
}

auto core::active_map() const noexcept -> const tilemap*
{
  if (m_activeMapIndex) {
    return m_maps.at(*m_activeMapIndex).get();
  } else {
    return nullptr;
  }
}

}  // namespace tactile::model

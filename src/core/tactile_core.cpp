#include "tactile_core.hpp"

#include <QImage>

#include "tilemap.hpp"
#include "tileset.hpp"
#include "tileset_manager.hpp"

namespace tactile {

TactileCore::TactileCore() : m_sheetManager{tileset_manager::unique()}
{}

TactileCore::~TactileCore() noexcept = default;

auto TactileCore::unique() -> std::unique_ptr<TactileCore>
{
  return std::make_unique<TactileCore>();
}

void TactileCore::new_map(int id) noexcept
{
  if (m_maps.count(id)) {
    qWarning("Editor core already had tilemap associated with %i!", id);
  } else {
    m_maps.insert({id, tilemap::unique(5, 5)});
    m_activeMapIndex = id;
  }
}

void TactileCore::close_map(int id) noexcept
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

void TactileCore::open_map(const char*)
{
  // TODO parse Tactile or TMX formats
  qWarning("\"Open map\" isn't implemented!");
}

void TactileCore::save_as(const char*) const
{
  qWarning("\"Save as\" isn't implemented!");
}

auto TactileCore::add_tileset(const std::shared_ptr<QImage>& image,
                              int tileWidth,
                              int tileHeight) noexcept -> std::optional<int>
{
  if (!image->isNull()) {
    return m_sheetManager->add(tileset::unique(image, tileWidth, tileHeight));
  } else {
    return std::nullopt;
  }
}

void TactileCore::set_rows(int nRows) noexcept
{
  if (auto* map = active_map(); map) {
    map->set_rows(nRows);
  }
}

void TactileCore::set_cols(int nCols) noexcept
{
  if (auto* map = active_map(); map) {
    map->set_cols(nCols);
  }
}

auto TactileCore::rows() const noexcept -> std::optional<int>
{
  return maybe_get<int>([](const tilemap& map) noexcept { return map.rows(); });
}

auto TactileCore::cols() const noexcept -> std::optional<int>
{
  return maybe_get<int>([](const tilemap& map) noexcept { return map.cols(); });
}

auto TactileCore::map_width() const noexcept -> std::optional<int>
{
  return maybe_get<int>(
      [](const tilemap& map) noexcept { return map.width(); });
}

auto TactileCore::map_height() const noexcept -> std::optional<int>
{
  return maybe_get<int>(
      [](const tilemap& map) noexcept { return map.height(); });
}

auto TactileCore::tile_size() const noexcept -> std::optional<int>
{
  return maybe_get<int>(
      [](const tilemap& map) noexcept { return map.get_tile_size().get(); });
}

void TactileCore::select_layer(int index) noexcept
{
  if (auto* map = active_map(); map) {
    map->select(index);
    emit s_updated();
  }
}

void TactileCore::select_map(int id) noexcept
{
  if (m_maps.count(id)) {
    m_activeMapIndex = id;
  }
}

void TactileCore::increase_tile_size() noexcept
{
  if (auto* map = active_map(); map) {
    map->get_tile_size().increase();
    emit s_updated();
  }
}

void TactileCore::decrease_tile_size() noexcept
{
  if (auto* map = active_map(); map) {
    map->get_tile_size().decrease();
    emit s_updated();
  }
}

void TactileCore::reset_tile_size() noexcept
{
  if (auto* map = active_map(); map) {
    map->get_tile_size().reset();
    emit s_updated();
  }
}

void TactileCore::draw(QPainter& painter) const noexcept
{
  if (auto* map = active_map(); map) {
    map->draw(painter);
  }
}

void TactileCore::add_row() noexcept
{
  if (auto* map = active_map(); map) {
    map->add_row();
    emit s_updated();
  }
}

void TactileCore::add_col() noexcept
{
  if (auto* map = active_map(); map) {
    map->add_col();
    emit s_updated();
  }
}

void TactileCore::remove_row() noexcept
{
  if (auto* map = active_map(); map) {
    map->remove_row();
    emit s_updated();
  }
}

void TactileCore::remove_col() noexcept
{
  if (auto* map = active_map(); map) {
    map->remove_col();
    emit s_updated();
  }
}

auto TactileCore::active_map() noexcept -> tilemap*
{
  if (m_activeMapIndex) {
    return m_maps.at(*m_activeMapIndex).get();
  } else {
    return nullptr;
  }
}

auto TactileCore::active_map() const noexcept -> const tilemap*
{
  if (m_activeMapIndex) {
    return m_maps.at(*m_activeMapIndex).get();
  } else {
    return nullptr;
  }
}

}  // namespace tactile

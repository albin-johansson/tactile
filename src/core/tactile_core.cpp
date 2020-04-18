#include "tactile_core.h"

#include <QImage>

#include "core/tileset/tileset.h"
#include "core/tileset/tileset_manager.h"
#include "tilemap.h"

namespace tactile {

TactileCore::TactileCore()
    : m_activeMapIndex{nothing},
      m_sheetManager{std::make_unique<TilesetManager>()}
{}

TactileCore::~TactileCore() noexcept = default;

void TactileCore::new_map(int id) noexcept
{
  if (m_maps.count(id)) {
    qWarning("Editor core already had tilemap associated with %i!", id);
  } else {
    m_maps.insert({id, Tilemap::unique(5, 5)});
    m_activeMapIndex = id;
  }
}

void TactileCore::close_map(int id) noexcept
{
  if (m_maps.count(id)) {
    m_maps.erase(id);

    if (m_activeMapIndex == id) {
      m_activeMapIndex = nothing;
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

Maybe<int> TactileCore::add_tile_sheet(const Shared<QImage>& image,
                                       int tileWidth,
                                       int tileHeight) noexcept
{
  if (!image->isNull()) {
    return m_sheetManager->add(Tileset::unique(image, tileWidth, tileHeight));
  } else {
    return nothing;
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

Maybe<int> TactileCore::rows() const noexcept
{
  return maybe_get<int>([](const Tilemap& map) noexcept { return map.rows(); });
}

Maybe<int> TactileCore::cols() const noexcept
{
  return maybe_get<int>([](const Tilemap& map) noexcept { return map.cols(); });
}

Maybe<int> TactileCore::map_width() const noexcept
{
  return maybe_get<int>(
      [](const Tilemap& map) noexcept { return map.width(); });
}

Maybe<int> TactileCore::map_height() const noexcept
{
  return maybe_get<int>(
      [](const Tilemap& map) noexcept { return map.height(); });
}

Maybe<int> TactileCore::tile_size() const noexcept
{
  return maybe_get<int>(
      [](const Tilemap& map) noexcept { return map.tile_size().size(); });
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
    map->tile_size().increase();
    emit s_updated();
  }
}

void TactileCore::decrease_tile_size() noexcept
{
  if (auto* map = active_map(); map) {
    map->tile_size().decrease();
    emit s_updated();
  }
}

void TactileCore::reset_tile_size() noexcept
{
  if (auto* map = active_map(); map) {
    map->tile_size().reset();
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

Tilemap* TactileCore::active_map() noexcept
{
  if (m_activeMapIndex) {
    return m_maps.at(*m_activeMapIndex).get();
  } else {
    return nullptr;
  }
}

const Tilemap* TactileCore::active_map() const noexcept
{
  if (m_activeMapIndex) {
    return m_maps.at(*m_activeMapIndex).get();
  } else {
    return nullptr;
  }
}

}  // namespace tactile

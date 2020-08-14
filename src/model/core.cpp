#include "core.hpp"

#include <QImage>

#include "render_tilemap.hpp"
#include "tilemap.hpp"
#include "tileset.hpp"
#include "tileset_manager.hpp"

namespace tactile::model {

void core::handle_new_map(int id) noexcept
{
  if (m_maps.count(id)) {
    qWarning("Editor core already had tilemap associated with %i!", id);
  } else {
    m_maps.insert({id, tilemap{5, 5}});
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

auto core::add_tileset(const QImage& image,
                       int tileWidth,
                       int tileHeight) noexcept -> std::optional<int>
{
  if (!image.isNull()) {
    return m_sheetManager.emplace(image, tileWidth, tileHeight);
  } else {
    return std::nullopt;
  }
}

void core::set_rows(int nRows) noexcept
{
  if (has_active_map()) {
    active_map().set_rows(nRows);
  }
}

void core::set_cols(int nCols) noexcept
{
  if (has_active_map()) {
    active_map().set_cols(nCols);
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
  if (has_active_map()) {
    active_map().select(index);
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
  if (has_active_map()) {
    active_map().get_tile_size().increase();
    emit request_update();
  }
}

void core::handle_decrease_tile_size() noexcept
{
  if (has_active_map()) {
    active_map().get_tile_size().decrease();
    emit request_update();
  }
}

void core::handle_reset_tile_size() noexcept
{
  if (has_active_map()) {
    active_map().get_tile_size().reset();
    emit request_update();
  }
}

//void core::handle_draw(QPainter& painter) const noexcept
//{
//  if (has_active_map()) {
//    active_map().draw(painter);
//  }
//}

void core::handle_draw(QPainter& painter, const QRectF& exposed)
{
  if (has_active_map()) {
    render_tilemap(painter, active_map(), exposed);
  }
}

void core::handle_add_row() noexcept
{
  if (has_active_map()) {
    active_map().add_row();
    emit request_update();
  }
}

void core::handle_add_col() noexcept
{
  if (has_active_map()) {
    active_map().add_col();
    emit request_update();
  }
}

void core::handle_remove_row() noexcept
{
  if (has_active_map()) {
    active_map().remove_row();
    emit request_update();
  }
}

void core::handle_remove_col() noexcept
{
  if (has_active_map()) {
    active_map().remove_col();
    emit request_update();
  }
}

auto core::active_map() noexcept -> tilemap&
{
  return m_maps.at(*m_activeMapIndex);
}

auto core::active_map() const noexcept -> const tilemap&
{
  return m_maps.at(*m_activeMapIndex);
}

}  // namespace tactile::model

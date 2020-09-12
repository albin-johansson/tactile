#include "tileset_manager.hpp"

#include "tactile_error.hpp"

namespace tactile::core {

tileset_manager::tileset_manager()
{
  m_tilesets.reserve(5);
}

void tileset_manager::remove(tileset_id id) noexcept
{
  m_tilesets.erase(id);
  if (id == m_activeID) {
    m_activeID = std::nullopt;
  }
}

void tileset_manager::remove_all() noexcept
{
  m_tilesets.clear();
}

void tileset_manager::select(std::optional<tileset_id> id)
{
  if (id) {
    Q_ASSERT(m_tilesets.contains(*id));
    m_activeID = id;
  } else {
    m_activeID = std::nullopt;
  }
}

void tileset_manager::set_selection(const position& topLeft,
                                  const position& bottomRight)
{
  if (has_active_tileset()) {
    m_tilesets.at(*m_activeID).set_selection(topLeft, bottomRight);
  }
}

auto tileset_manager::at(tileset_id id) -> tileset&
{
  return m_tilesets.at(id);
}

auto tileset_manager::at(tileset_id id) const -> const tileset&
{
  return m_tilesets.at(id);
}

auto tileset_manager::image(tile_id id) const -> const QPixmap&
{
  for (const auto& [key, tileset] : m_tilesets) {
    if (tileset.contains(id)) {
      return tileset.image();
    }
  }
  throw tactile_error{"tileset_model > failed to find image for tile ID!"};
}

auto tileset_manager::image_source(tile_id id) const -> QRect
{
  // TODO optimize by storing the source rectangles
  for (const auto& [key, tileset] : m_tilesets) {
    if (tileset.contains(id)) {
      const auto index = id - tileset.first_id();

      const auto row = index.get() / tileset.cols();
      const auto col = index.get() % tileset.cols();

      const auto tileWidth = tileset.get_tile_width().get();
      const auto tileHeight = tileset.get_tile_height().get();

      const auto x = col * tileset.get_tile_width().get();
      const auto y = row * tileset.get_tile_height().get();

      return {x, y, tileWidth, tileHeight};
    }
  }
  throw tactile_error{"tileset_model > failed to obtain source rectangle!"};
}

auto tileset_manager::range_of(tileset_id id) const -> std::pair<tile_id, tile_id>
{
  const auto& tileset = m_tilesets.at(id);
  return {tileset.first_id(), tileset.last_id()};
}

auto tileset_manager::sheets() const noexcept -> int
{
  return static_cast<int>(m_tilesets.size());
}

auto tileset_manager::has_active_tileset() const noexcept -> bool
{
  return m_activeID.has_value();
}

auto tileset_manager::current_tileset() const -> const tileset*
{
  return m_activeID ? &m_tilesets.at(*m_activeID) : nullptr;
}

auto tileset_manager::current_tileset_id() const -> std::optional<tileset_id>
{
  return m_activeID;
}

auto tileset_manager::contains(tile_id id) const -> bool
{
  return std::any_of(
      m_tilesets.begin(), m_tilesets.end(), [id](const auto& pair) {
        return pair.second.contains(id);
      });
}

}  // namespace tactile::core

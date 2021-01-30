#include "tileset_manager.hpp"

#include <ranges>  // any_of

#include "tactile_error.hpp"

namespace tactile::core {

tileset_manager::tileset_manager()
{
  m_tilesets.reserve(5);
}

void tileset_manager::add(const tileset_id id, shared<tileset> tileset)
{
  Q_ASSERT(!contains(id));
  Q_ASSERT(tileset);

  m_nextGlobalTileId = tileset->last_id() + 1_t;
  m_tilesets.emplace(id, std::move(tileset));
  m_activeId = id;
}

auto tileset_manager::add(shared<tileset> tileset) -> tileset_id
{
  Q_ASSERT(tileset);

  const auto id = m_nextId;
  ++m_nextId;

  add(id, std::move(tileset));
  return id;
}

void tileset_manager::remove(const tileset_id id) noexcept
{
  // TODO consider decrementing m_nextID and decreasing m_nextGlobalTileID
  m_tilesets.erase(id);

  if (id == m_activeId) {
    m_activeId = std::nullopt;
  }
}

void tileset_manager::remove_all() noexcept
{
  m_tilesets.clear();
}

void tileset_manager::rename(const tileset_id id, const QString& name)
{
  m_tilesets.at(id)->set_name(name);
}

void tileset_manager::select(const maybe<tileset_id> id)
{
  if (id) {
    Q_ASSERT(m_tilesets.contains(*id));
    m_activeId = id;
  } else {
    m_activeId = std::nullopt;
  }
}

void tileset_manager::set_selection(const tileset::selection& selection)
{
  if (has_active_tileset()) {
    m_tilesets.at(*m_activeId)->set_selection(selection);
  }
}

void tileset_manager::increment_next_tileset_id() noexcept
{
  ++m_nextId;
}

auto tileset_manager::at(const tileset_id id) -> tileset&
{
  return *m_tilesets.at(id);
}

auto tileset_manager::at(const tileset_id id) const -> const tileset&
{
  return *m_tilesets.at(id);
}

auto tileset_manager::image(const tile_id id) const -> const QPixmap&
{
  for (const auto& [key, tileset] : m_tilesets) {
    if (tileset->contains(id)) {
      return tileset->image();
    }
  }
  throw tactile_error{"tileset_manager > failed to find image for tile ID!"};
}

auto tileset_manager::image_source(const tile_id id) const -> QRect
{
  for (const auto& [key, tileset] : m_tilesets) {
    if (const auto rect = tileset->image_source(id)) {
      return *rect;
    }
  }
  throw tactile_error{"tileset_manager > failed to obtain source rectangle!"};
}

auto tileset_manager::range_of(const tileset_id id) const -> tile_range
{
  const auto& tileset = m_tilesets.at(id);
  return {tileset->first_id(), tileset->last_id()};
}

auto tileset_manager::count() const noexcept -> int
{
  return static_cast<int>(m_tilesets.size());
}

auto tileset_manager::has_active_tileset() const noexcept -> bool
{
  return m_activeId.has_value();
}

auto tileset_manager::current_tileset() const -> const tileset*
{
  return m_activeId ? m_tilesets.at(*m_activeId).get() : nullptr;
}

auto tileset_manager::current_tileset_id() const -> maybe<tileset_id>
{
  return m_activeId;
}

auto tileset_manager::contains(const tile_id id) const -> bool
{
  return std::ranges::any_of(m_tilesets, [id](const auto& pair) {
    return pair.second->contains(id);
  });
}

auto tileset_manager::contains(const tileset_id id) const -> bool
{
  return m_tilesets.contains(id);
}

}  // namespace tactile::core

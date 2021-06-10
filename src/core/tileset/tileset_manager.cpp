#include "tileset_manager.hpp"

#include <ranges>   // any_of
#include <utility>  // move

#include "tactile_error.hpp"

namespace tactile::core {

tileset_manager::tileset_manager()
{
  m_tilesets.reserve(5);
}

void tileset_manager::add(const tileset_id id, shared<Tileset> tileset)
{
  Q_ASSERT(!contains(id));
  Q_ASSERT(tileset);

  m_nextGlobalTileId = tileset->LastId() + 1_t;
  m_tilesets.emplace(id, std::move(tileset));
  m_activeId = id;
}

auto tileset_manager::add(shared<Tileset> tileset) -> tileset_id
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

  if (id == m_activeId)
  {
    m_activeId = std::nullopt;
  }
}

void tileset_manager::clear() noexcept
{
  m_tilesets.clear();
}

void tileset_manager::rename(const tileset_id id, const QString& name)
{
  m_tilesets.at(id)->SetName(name);
}

void tileset_manager::select(const maybe<tileset_id> id)
{
  if (id)
  {
    Q_ASSERT(m_tilesets.contains(*id));
    m_activeId = id;
  }
  else
  {
    m_activeId = std::nullopt;
  }
}

void tileset_manager::set_selection(const tileset_selection& selection)
{
  if (has_active_tileset())
  {
    m_tilesets.at(*m_activeId)->SetSelection(selection);
  }
}

void tileset_manager::increment_next_tileset_id() noexcept
{
  ++m_nextId;
}

auto tileset_manager::at(const tileset_id id) -> Tileset&
{
  return *m_tilesets.at(id);
}

auto tileset_manager::at(const tileset_id id) const -> const Tileset&
{
  return *m_tilesets.at(id);
}

auto tileset_manager::image(const tile_id id) const -> const QPixmap&
{
  for (const auto& [key, tileset] : m_tilesets)
  {
    if (tileset->Contains(id))
    {
      return tileset->Image();
    }
  }
  throw TactileError{"tileset_manager > failed to find image for tile ID!"};
}

auto tileset_manager::image_source(const tile_id id) const -> QRect
{
  for (const auto& [key, tileset] : m_tilesets)
  {
    if (const auto rect = tileset->ImageSource(id))
    {
      return *rect;
    }
  }
  throw TactileError{"tileset_manager > failed to obtain source rectangle!"};
}

auto tileset_manager::range_of(const tileset_id id) const -> tile_range
{
  Q_ASSERT(contains(id));

  const auto& tileset = m_tilesets.at(id);
  return {tileset->FirstId(), tileset->LastId()};
}

auto tileset_manager::count() const noexcept -> int
{
  return static_cast<int>(m_tilesets.size());
}

auto tileset_manager::has_active_tileset() const noexcept -> bool
{
  return m_activeId.has_value();
}

auto tileset_manager::current_tileset() const -> const Tileset*
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
    return pair.second->Contains(id);
  });
}

auto tileset_manager::contains(const tileset_id id) const -> bool
{
  return m_tilesets.contains(id);
}

}  // namespace tactile::core

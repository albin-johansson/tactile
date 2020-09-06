#include "tileset_model.hpp"

#include "tileset.hpp"

namespace tactile::core {

void tileset_model::remove(tileset_id id) noexcept
{
  m_tilesets.erase(id);
  if (id == m_activeID) {
    m_activeID = std::nullopt;
    qDebug("tileset_model > no current active tileset");
  }
}

void tileset_model::remove_all() noexcept
{
  m_tilesets.clear();
}

void tileset_model::select(std::optional<tileset_id> id)
{
  if (id) {
    Q_ASSERT(m_tilesets.contains(*id));
    m_activeID = id;
    qDebug("tileset_model > the active tileset is now %i", id->get());
  } else {
    m_activeID = std::nullopt;
  }
}

void tileset_model::update_selection(position topLeft, position bottomRight)
{
  if (has_active_tileset()) {
    m_tilesets.at(*m_activeID).set_selection(topLeft, bottomRight);
  }
}

auto tileset_model::sheets() const noexcept -> int
{
  return static_cast<int>(m_tilesets.size());
}

auto tileset_model::has_active_tileset() const noexcept -> bool
{
  return m_activeID.has_value();
}

auto tileset_model::current_tileset() const -> const tileset*
{
  return m_activeID ? &m_tilesets.at(*m_activeID) : nullptr;
}

}  // namespace tactile::core

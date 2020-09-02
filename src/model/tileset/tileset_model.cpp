#include "tileset_model.hpp"

#include "tileset.hpp"

namespace tactile::model {

void tileset_model::remove(tileset_id id) noexcept
{
  m_tilesets.erase(id);
}

void tileset_model::remove_all() noexcept
{
  m_tilesets.clear();
}

void tileset_model::select(std::optional<tileset_id> id)
{
  if (!id) {
    m_activeID = std::nullopt;
  } else {
    if (m_tilesets.count(*id)) {
      m_activeID = id;
    }
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

}  // namespace tactile::model

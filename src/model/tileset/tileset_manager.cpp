#include "tileset_manager.hpp"

#include "tileset.hpp"

namespace tactile::model {

void tileset_manager::remove(int id) noexcept
{
  m_sheets.erase(id);
}

void tileset_manager::remove_all() noexcept
{
  m_sheets.clear();
}

void tileset_manager::select(std::optional<int> id) noexcept
{
  if (!id) {
    m_activeSheet = std::nullopt;
  } else {
    if (m_sheets.count(*id)) {
      m_activeSheet = id;
    }
  }
}

auto tileset_manager::sheets() const noexcept -> int
{
  return static_cast<int>(m_sheets.size());
}

auto tileset_manager::has_active_tileset() const noexcept -> bool
{
  return m_activeSheet.has_value();
}

}  // namespace tactile::model

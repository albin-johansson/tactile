#include "tileset_manager.h"

#include "tileset.h"

namespace tactile {

TilesetManager::TilesetManager() noexcept
    : m_activeSheet{nothing}, m_nextSheetKey{1}
{}

TilesetManager::~TilesetManager() noexcept = default;

Maybe<int> TilesetManager::add(Unique<Tileset>&& sheet) noexcept
{
  if (sheet) {
    const auto id = m_nextSheetKey++;
    m_sheets.emplace(id, std::move(sheet));
    return id;
  } else {
    return nothing;
  }
}

void TilesetManager::remove(int id) noexcept
{
  m_sheets.erase(id);
}

void TilesetManager::remove_all() noexcept
{
  m_sheets.clear();
}

void TilesetManager::select(Maybe<int> id) noexcept
{
  if (!id) {
    m_activeSheet = nothing;
  } else {
    if (m_sheets.count(*id)) {
      m_activeSheet = id;
    }
  }
}

}  // namespace tactile

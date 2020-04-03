#include "tile_sheet_manager.h"

#include "tile_sheet.h"

namespace tactile {

TileSheetManager::TileSheetManager() noexcept
    : m_activeSheet{nothing}, m_nextSheetKey{1}
{}

TileSheetManager::~TileSheetManager() noexcept = default;

Maybe<int> TileSheetManager::add(Unique<TileSheet>&& sheet) noexcept
{
  if (sheet) {
    const auto id = m_nextSheetKey++;
    m_sheets.emplace(id, std::move(sheet));
    return id;
  } else {
    return nothing;
  }
}

void TileSheetManager::remove(int id) noexcept
{
  m_sheets.erase(id);
}

void TileSheetManager::remove_all() noexcept
{
  m_sheets.clear();
}

void TileSheetManager::select(Maybe<int> id) noexcept
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

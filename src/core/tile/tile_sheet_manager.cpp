#include "tile_sheet_manager.h"

#include "tile_sheet.h"

namespace tactile {

TileSheetManager::TileSheetManager() noexcept : m_activeSheet{nothing}
{}

TileSheetManager::~TileSheetManager() noexcept = default;

void TileSheetManager::add(int id, UniquePtr<TileSheet>&& sheet) noexcept
{
  if (sheet) {
    m_sheets.emplace(id, std::move(sheet));
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

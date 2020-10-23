#include "tileset_tab_manager.hpp"

#include "tileset_tab.hpp"

namespace tactile::gui {

void tileset_tab_manager::add(tileset_id id, tileset_tab* tab)
{
  Q_ASSERT(tab);
  Q_ASSERT(tab->parent());  // require that the tab has an owner
  m_tabs.emplace(id, tab);
}

void tileset_tab_manager::remove(tileset_id id)
{
  //  auto* tab = m_tabs.at(id);
  m_tabs.erase(id);
  //  delete tab;
}

void tileset_tab_manager::clear() noexcept
{
  //  for (const auto& [id, tab] : m_tabs) {
  //    delete tab;
  //  }
  m_tabs.clear();
}

void tileset_tab_manager::set_cached_index(int index)
{
  m_cachedIndex = index;
}

auto tileset_tab_manager::at(tileset_id id) const -> const tileset_tab*
{
  return m_tabs.at(id);
}

auto tileset_tab_manager::index_of(tileset_id id) const -> int
{
  return m_tabs.index_of(id).value();
}

auto tileset_tab_manager::contains(tileset_id id) const -> bool
{
  return m_tabs.contains(id);
}

auto tileset_tab_manager::is_empty() const -> bool
{
  return m_tabs.empty();
}

}  // namespace tactile::gui

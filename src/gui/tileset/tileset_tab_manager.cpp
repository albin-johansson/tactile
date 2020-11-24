#include "tileset_tab_manager.hpp"

#include "tileset_tab.hpp"

namespace tactile::gui {

void tileset_tab_manager::add(const tileset_id id, tileset_tab* tab)
{
  Q_ASSERT(tab);
  Q_ASSERT(tab->parent());  // require that the tab has an owner
  m_tabs.emplace(id, tab);
}

void tileset_tab_manager::remove(const tileset_id id)
{
  m_tabs.erase(id);
}

void tileset_tab_manager::clear() noexcept
{
  m_tabs.clear();
}

void tileset_tab_manager::set_cached_index(const int index)
{
  m_cachedIndex = index;
}

auto tileset_tab_manager::at(const tileset_id id) const -> const tileset_tab*
{
  return m_tabs.at(id);
}

auto tileset_tab_manager::index_of(const tileset_id id) const -> int
{
  return static_cast<int>(m_tabs.index_of(id).value());
}

auto tileset_tab_manager::contains(const tileset_id id) const -> bool
{
  return m_tabs.contains(id);
}

auto tileset_tab_manager::is_empty() const -> bool
{
  return m_tabs.empty();
}

}  // namespace tactile::gui

#include "tileset_tab_context_menu.hpp"

#include "icons.hpp"

namespace tactile::gui {

tileset_tab_context_menu::tileset_tab_context_menu(QWidget* parent)
    : QMenu{parent},
      m_rename{addAction(icons::rename(), tr("Rename tileset"))},
      m_sep0{addSeparator()},
      m_remove{addAction(icons::remove(),tr("Remove tileset"))}
{
  connect(m_rename, &QAction::triggered, [this] {
    emit rename(m_index);
  });
  connect(m_remove, &QAction::triggered, [this] {
    emit remove(m_index);
  });
}

void tileset_tab_context_menu::set_tab_index(int index)
{
  m_index = index;
}

}  // namespace tactile::gui

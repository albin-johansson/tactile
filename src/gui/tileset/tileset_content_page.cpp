#include "tileset_content_page.hpp"

#include <QPushButton>
#include <QTabBar>

#include "icons.hpp"
#include "signal_blocker.hpp"
#include "tileset_tab.hpp"
#include "tileset_tab_context_menu.hpp"
#include "ui_tileset_content_page.h"

namespace tactile::gui {

tileset_content_page::tileset_content_page(QWidget* parent)
    : QWidget{parent},
      m_ui{new Ui::tileset_content_page{}}
{
  m_ui->setupUi(this);

  m_contextMenu = new tileset_tab_context_menu{this};
//  connect(m_contextMenu, &tileset_tab_context_menu::rename, [this](int index) {
//    // TODO
//  });

  connect(m_contextMenu, &tileset_tab_context_menu::remove, [this](int index) {
    if (auto* tab = tab_from_index(index)) {
      emit ui_remove_tileset(tab->id());
    }
  });

  add_corner_button();

  connect(m_ui->tabWidget,
          &QTabWidget::customContextMenuRequested,
          this,
          &tileset_content_page::trigger_context_menu);

  connect(m_ui->tabWidget, &QTabWidget::tabCloseRequested, [this](int index) {
    if (auto* tab = tab_from_index(index)) {
      emit ui_remove_tileset(tab->id());
    }
  });

  connect(m_ui->tabWidget, &QTabWidget::tabBarClicked, [this](int index) {
    if (auto* tab = tab_from_index(index)) {
      emit ui_select_tileset(tab->id());
    }
  });

  connect(m_ui->tabWidget, &QTabWidget::currentChanged, [this](int index) {
    if (index != -1) {
      current_manager().set_cached_index(index);
      if (auto* item = tab_from_index(index)) {
        emit ui_select_tileset(item->id());
      }
    } else {
      emit switch_to_empty_page();
    }
  });
}

tileset_content_page::~tileset_content_page() noexcept
{
  delete m_ui;
}

void tileset_content_page::add_corner_button()
{
  auto* button = new QPushButton{m_ui->tabWidget};
  button->setIcon(icons::add());

  connect(button,
          &QPushButton::pressed,
          this,
          &tileset_content_page::ui_add_tileset);

  m_ui->tabWidget->setCornerWidget(button, Qt::Corner::TopRightCorner);
}

void tileset_content_page::trigger_context_menu(const QPoint& pos)
{
  if (const auto index = m_ui->tabWidget->tabBar()->tabAt(pos); index != -1) {
    m_contextMenu->set_tab_index(index);
    m_contextMenu->exec(mapToGlobal(pos));
  }
}

void tileset_content_page::selected_map(map_id map)
{
  {
    signal_blocker blocker{m_ui->tabWidget};  // avoid `ui_select_tileset`

    if (m_currentMap) {
      m_ui->tabWidget->clear();
    }

    switch_to(map);
  }

  if (current_manager().is_empty()) {
    emit switch_to_empty_page();
  } else {
    emit switch_to_content_page();
  }
}

void tileset_content_page::added_tileset(map_id map,
                                         tileset_id id,
                                         const core::tileset& tileset)
{
  Q_ASSERT(!current_manager().contains(id));

  auto* tab = new tileset_tab{id, tileset, m_ui->tabWidget};
  connect(tab,
          &tileset_tab::set_tileset_selection,
          this,
          &tileset_content_page::ui_set_tileset_selection);
  m_tabManagers.at(map).add(id, tab);

  const auto index = m_ui->tabWidget->addTab(tab, tileset.name());
  m_ui->tabWidget->setCurrentIndex(index);
}

void tileset_content_page::removed_tileset(tileset_id id)
{
  auto& manager = current_manager();
  Q_ASSERT(manager.contains(id));

  m_ui->tabWidget->removeTab(manager.index_of(id));
  manager.remove(id);
}

auto tileset_content_page::is_empty() const -> bool
{
  return m_ui->tabWidget->count() == 0;
}

void tileset_content_page::switch_to(map_id map)
{
  m_currentMap = map;
  const auto& manager = m_tabManagers[*m_currentMap];

  for (const auto& [_, tab] : manager) {
    m_ui->tabWidget->addTab(tab, tab->name());
  }

  m_ui->tabWidget->setCurrentIndex(manager.cached_index().value_or(0));
}

auto tileset_content_page::tab_from_index(int index) -> tileset_tab*
{
  return qobject_cast<tileset_tab*>(m_ui->tabWidget->widget(index));
}

auto tileset_content_page::current_manager() -> tileset_tab_manager&
{
  return m_tabManagers.at(m_currentMap.value());
}

}  // namespace tactile::gui

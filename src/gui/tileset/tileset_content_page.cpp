#include "tileset_content_page.hpp"

#include <qpushbutton.h>

#include <algorithm>  // find_if

#include "tileset_tab.hpp"
#include "ui_tileset_content_page.h"

namespace tactile::gui {

tileset_content_page::tileset_content_page(QWidget* parent)
    : QWidget{parent},
      m_ui{new Ui::tileset_content_page{}}
{
  m_ui->setupUi(this);

  auto* cornerBtn = new QPushButton{m_ui->tabWidget};
  cornerBtn->setIcon(
      QIcon{QStringLiteral(u":resources/icons/icons8/color/64/add.png")});
  connect(cornerBtn,
          &QPushButton::pressed,
          this,
          &tileset_content_page::add_new_tileset);
  m_ui->tabWidget->setCornerWidget(cornerBtn, Qt::Corner::TopRightCorner);

  connect(m_ui->tabWidget,
          &QTabWidget::tabCloseRequested,
          this,
          &tileset_content_page::handle_remove_tab);

  connect(m_ui->tabWidget,
          &QTabWidget::currentChanged,
          this,
          &tileset_content_page::handle_tab_changed);
}

tileset_content_page::~tileset_content_page() noexcept
{
  delete m_ui;
}

void tileset_content_page::add_tileset(map_id map,
                                       tileset_id id,
                                       const core::tileset& tileset)
{
  Q_ASSERT(!has_tab(id));

  auto* tab = new tileset_tab{id, tileset, this};
  connect(tab,
          &tileset_tab::tileset_selection_changed,
          this,
          &tileset_content_page::tileset_selection_changed);
  m_mapTabs.at(map).emplace(id, tab);
  const auto index = m_ui->tabWidget->addTab(tab, tileset.name());
  m_ui->tabWidget->setCurrentIndex(index);
}

void tileset_content_page::remove_tileset(tileset_id id)
{
  Q_ASSERT(has_tab(id));

  const auto index = index_of(id);
  Q_ASSERT(index);

  current_tab().erase(id);

  m_ui->tabWidget->removeTab(*index);
  emit removed_tileset(id);
}

auto tileset_content_page::empty() const -> bool
{
  return m_ui->tabWidget->count() == 0;
}

void tileset_content_page::selected_map(map_id map)
{
  if (m_currentMap) {
    m_ui->tabWidget->clear();
  }

  m_currentMap = map;

  if (!m_mapTabs.contains(map)) {
    m_mapTabs.emplace(map, std::map<tileset_id, tileset_tab*>{});
  } else {
    for (const auto& [key, value] : m_mapTabs.at(map)) {
      m_ui->tabWidget->addTab(value, QStringLiteral(u"Foo"));
    }
    m_ui->tabWidget->setCurrentIndex(0);
  }

  if (current_tab().empty()) {
    emit switch_to_empty_page();
  } else {
    emit switch_to_content_page();
  }
}

auto tileset_content_page::has_tab(tileset_id id) const -> bool
{
  const auto& current = current_tab();
  return current.find(id) != current.end();
}

auto tileset_content_page::tab_from_index(int index) -> tileset_tab*
{
  return qobject_cast<tileset_tab*>(m_ui->tabWidget->widget(index));
}

auto tileset_content_page::index_of(tileset_id id) const -> std::optional<int>
{
  int index{0};
  for (const auto& [tilesetID, tab] : current_tab()) {
    if (tilesetID == id) {
      return index;
    } else {
      ++index;
    }
  }

  return std::nullopt;
}

void tileset_content_page::handle_remove_tab(int index)
{
  if (auto* tab = tab_from_index(index)) {
    remove_tileset(tab->id());
  }
}

void tileset_content_page::handle_tab_changed(int index)
{
  if (index == -1) {
    emit switch_to_empty_page();
  } else {
    if (auto* tab = tab_from_index(index)) {
      emit selected_tileset(tab->id());
    }
  }
}

}  // namespace tactile::gui

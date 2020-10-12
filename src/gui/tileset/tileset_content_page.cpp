#include "tileset_content_page.hpp"

#include <QPushButton>

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
          &tileset_content_page::ui_requested_tileset);
  m_ui->tabWidget->setCornerWidget(cornerBtn, Qt::Corner::TopRightCorner);

  connect(m_ui->tabWidget,
          &QTabWidget::tabCloseRequested,
          this,
          &tileset_content_page::handle_remove_tab);
  connect(m_ui->tabWidget,
          &QTabWidget::currentChanged,
          this,
          &tileset_content_page::handle_tab_changed);
  connect(m_ui->tabWidget,
          &QTabWidget::tabBarClicked,
          this,
          &tileset_content_page::handle_tab_clicked);
}

tileset_content_page::~tileset_content_page() noexcept
{
  delete m_ui;
}

void tileset_content_page::add_tileset(map_id map,
                                       tileset_id id,
                                       const core::tileset& tileset)
{
  Q_ASSERT(!current_tab().contains(id));

  auto* tab = new tileset_tab{id, tileset, this};
  connect(tab,
          &tileset_tab::tileset_selection_changed,
          this,
          &tileset_content_page::tileset_selection_changed);
  m_tabData.at(map).add(id, tab);

  const auto index = m_ui->tabWidget->addTab(tab, tileset.name());
  m_ui->tabWidget->setCurrentIndex(index);
}

void tileset_content_page::remove_tileset(tileset_id id, bool notify)
{
  Q_ASSERT(current_tab().contains(id));

  const auto index = index_of(id).value();

  current_tab().erase(id);

  m_ui->tabWidget->removeTab(index);

  if (notify) {
    emit ui_removed_tileset(id);
  }
}

auto tileset_content_page::empty() const -> bool
{
  return m_ui->tabWidget->count() == 0;
}

void tileset_content_page::select_map(map_id map)
{
  m_switchingMap = true;

  if (m_currentMap) {
    m_ui->tabWidget->clear();
  }

  switch_to(map);

  if (current_tab().is_empty()) {
    emit switch_to_empty_page();
  } else {
    emit switch_to_content_page();
  }

  m_switchingMap = false;
}

void tileset_content_page::switch_to(map_id map)
{
  m_currentMap = map;

  if (!m_tabData.contains(map)) {
    m_tabData.emplace(map, tab_data{});
  } else {
    const auto index = current_tab().cached_index().value_or(0);

    for (const auto& [key, tab] : m_tabData.at(map)) {
      m_ui->tabWidget->addTab(tab, tab->name());
    }

    m_ui->tabWidget->setCurrentIndex(index);
  }
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
  if (!m_switchingMap) {
    current_tab().set_cached_index(index);
  }

  if (index == -1) {
    emit switch_to_empty_page();
  }
}

void tileset_content_page::handle_tab_clicked(int index)
{
  if (auto* tab = tab_from_index(index)) {
    emit ui_selected_tileset(tab->id());
  }
}

}  // namespace tactile::gui

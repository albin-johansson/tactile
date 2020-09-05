#include "tileset_content_page.hpp"

#include <qpushbutton.h>

#include <algorithm>  // find_if

#include "tileset_tab.hpp"
#include "ui_tileset_content_page.h"

namespace tactile::gui {

tileset_content_page::tileset_content_page(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::tileset_content_page{}}
{
  m_ui->setupUi(this);

  m_tabs.reserve(5);

  auto* cornerBtn = new QPushButton{m_ui->tabWidget};
  cornerBtn->setIcon(QIcon{":resources/icons/icons8/color/64/add.png"});
  connect(cornerBtn,
          &QPushButton::pressed,
          this,
          &tileset_content_page::request_add_tileset);
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

void tileset_content_page::add_tileset(const QImage& image,
                                       tileset_id id,
                                       tile_width tileWidth,
                                       tile_height tileHeight,
                                       const QString& tabName)
{
  Q_ASSERT(!has_tab(id));

  if (!image.isNull()) {
    auto* tab = new tileset_tab{image, id, tileWidth, tileHeight, this};
    connect(tab,
            &tileset_tab::tileset_selection_changed,
            this,
            &tileset_content_page::tileset_selection_changed);
    m_tabs.push_back(tab);
    const auto index = m_ui->tabWidget->addTab(tab, tabName);
    m_ui->tabWidget->setCurrentIndex(index);
  }
}

void tileset_content_page::remove_tileset(tileset_id id)
{
  if (const auto it = find_tab(id); it != m_tabs.end()) {
    m_tabs.erase(it);
    m_ui->tabWidget->removeTab(m_ui->tabWidget->currentIndex());
    emit removed_tileset(id);
  }
}

auto tileset_content_page::empty() const -> bool
{
  return m_ui->tabWidget->count() == 0;
}

auto tileset_content_page::has_tab(tileset_id id) const -> bool
{
  return find_tab(id) != m_tabs.end();
}

auto tileset_content_page::find_tab(tileset_id id) const -> const_iterator
{
  const auto predicate = [id](const tileset_tab* tab) noexcept {
    return tab->id() == id;
  };
  return std::find_if(m_tabs.begin(), m_tabs.end(), predicate);
}

auto tileset_content_page::tab_from_index(int index) -> tileset_tab*
{
  return qobject_cast<tileset_tab*>(m_ui->tabWidget->widget(index));
}

void tileset_content_page::handle_remove_tab(int index)
{
  if (auto* tab = tab_from_index(index)) {
    const auto id = tab->id();
    remove_tileset(id);
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

#include "tileset_content_page.hpp"

#include <QPushButton>

#include "tileset_info.hpp"
#include "tileset_tab.hpp"
#include "ui_tileset_content_page.h"

namespace tactile::ui {

TilesetContentPage::TilesetContentPage(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::TilesetContentPageUI{}}
{
  m_ui->setupUi(this);

  //  auto* cornerBtn = new QPushButton{}; // deleted by tab widget
  //  m_ui->tabWidget->setCornerWidget(cornerBtn);
}

TilesetContentPage::~TilesetContentPage() noexcept
{
  delete m_ui;
}

void TilesetContentPage::add_tileset(const TilesetInfo& info,
                                     const QString& tabName) noexcept
{
  if (m_tabs.count(info.id)) {
    qDebug("Tried to add tileset with taken ID: %i", info.id);
  } else {
    if (!info.image->isNull()) {
      auto tilesetTab = std::make_shared<TilesetTab>(
          *info.image, info.tileWidth, info.tileHeight);
      m_ui->tabWidget->addTab(tilesetTab.get(), tabName);
      m_tabs.emplace(info.id, tilesetTab);
    }
  }
}

void TilesetContentPage::remove_tileset(int id) noexcept
{
  if (m_tabs.count(id)) {
    m_tabs.erase(id);
    m_ui->tabWidget->removeTab(m_ui->tabWidget->currentIndex());
  } else {
    qDebug("Tried to remove non-existent tileset, ID: %i", id);
  }
}

bool TilesetContentPage::empty() const noexcept
{
  return m_ui->tabWidget->count() == 0;
}

}  // namespace tactile::ui

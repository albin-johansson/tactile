#include "tileset_content_page.hpp"

#include <QPushButton>

#include "tileset_info.hpp"
#include "tileset_tab.hpp"
#include "ui_tileset_content_page.h"

using tactile::model::TilesetInfo;

namespace tactile::ui {

tileset_content_page::tileset_content_page(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::TilesetContentPageUI{}}
{
  m_ui->setupUi(this);

  //  auto* cornerBtn = new QPushButton{}; // deleted by tab widget
  //  m_ui->tabWidget->setCornerWidget(cornerBtn);
}

tileset_content_page::~tileset_content_page() noexcept
{
  delete m_ui;
}

void tileset_content_page::add_tileset(const TilesetInfo& info,
                                     const QString& tabName) noexcept
{
  if (m_tabs.count(info.id)) {
    qDebug("Tried to add tileset with taken ID: %i", info.id);
  } else {
    if (!info.image->isNull()) {
      auto tilesetTab = std::make_shared<tileset_tab>(
          *info.image, info.tileWidth, info.tileHeight);
      m_ui->tabWidget->addTab(tilesetTab.get(), tabName);
      m_tabs.emplace(info.id, tilesetTab);
    }
  }
}

void tileset_content_page::remove_tileset(int id) noexcept
{
  if (m_tabs.count(id)) {
    m_tabs.erase(id);
    m_ui->tabWidget->removeTab(m_ui->tabWidget->currentIndex());
  } else {
    qDebug("Tried to remove non-existent tileset, ID: %i", id);
  }
}

auto tileset_content_page::empty() const noexcept -> bool
{
  return m_ui->tabWidget->count() == 0;
}

}  // namespace tactile::ui

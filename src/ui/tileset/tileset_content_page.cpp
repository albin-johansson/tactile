#include "tileset_content_page.h"

#include <QPushButton>

#include "tileset_tab.h"
#include "ui_tileset_content_page.h"

namespace tactile {

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

void TilesetContentPage::add_tileset(int id,
                                     const Shared<QImage>& image,
                                     int tileWidth,
                                     int tileHeight) noexcept
{
  if (m_tabs.count(id)) {
    qDebug("Tried to add tileset with taken ID: %i", id);
  } else {
    if (!image->isNull()) {
      auto tilesetTab =
          std::make_shared<TilesetTab>(*image, tileWidth, tileHeight);

      // FIXME use file name
      m_ui->tabWidget->addTab(tilesetTab.get(), "untitled");
      m_tabs.emplace(id, tilesetTab);
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

}  // namespace tactile

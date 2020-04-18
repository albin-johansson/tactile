#include "tileset_content_page.h"

#include "tileset_tab.h"
#include "ui_tileset_content_page.h"

namespace tactile {

TilesetContentPage::TilesetContentPage(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::TilesetContentPageUI{}}
{
  m_ui->setupUi(this);
  m_ui->tabWidget->setMovable(false);

  connect(m_ui->newButton,
          &QPushButton::clicked,
          this,
          &TilesetContentPage::s_requested_tile_sheet);
}

TilesetContentPage::~TilesetContentPage() noexcept
{
  delete m_ui;
}

void TilesetContentPage::add_tile_sheet(int id,
                                          const Shared<QImage>& image) noexcept
{
  if (m_tabs.count(id)) {
    qDebug("Tried to add tileset with taken ID: %i", id);
  } else {
    if (!image->isNull()) {
      auto tilesetTab = std::make_shared<TilesetTab>(*image);

      m_ui->tabWidget->addTab(tilesetTab.get(), "untitled");
      m_tabs.emplace(id, tilesetTab);
    }
  }
}

void TilesetContentPage::remove_tile_sheet(int id) noexcept
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

#include "tile_sheet_content_page.h"

#include "tile_sheet_tab.h"
#include "ui_tile_sheet_content_page.h"

namespace tactile {

TileSheetContentPage::TileSheetContentPage(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::TileSheetContentPageUI{}}
{
  m_ui->setupUi(this);
}

TileSheetContentPage::~TileSheetContentPage() noexcept
{
  delete m_ui;
}

void TileSheetContentPage::add_tile_sheet(int id,
                                          const Shared<QImage>& image) noexcept
{
  if (m_tabs.count(id)) {
    qDebug("Tried to add tile sheet with taken ID: %i", id);
  } else {
    if (!image->isNull()) {
      auto tileSheetTab = std::make_shared<TileSheetTab>(*image);
      m_ui->tabWidget->addTab(tileSheetTab.get(), "untitled");
      m_tabs.emplace(id, tileSheetTab);
    }
  }
}

void TileSheetContentPage::remove_tile_sheet(int id) noexcept
{
  if (m_tabs.count(id)) {
    m_tabs.erase(id);
    m_ui->tabWidget->removeTab(m_ui->tabWidget->currentIndex());
  } else {
    qDebug("Tried to remove non-existent tile sheet, ID: %i", id);
  }
}

bool TileSheetContentPage::empty() const noexcept
{
  return m_ui->tabWidget->count() == 0;
}

}  // namespace tactile

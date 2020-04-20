#include "tileset_widget.h"

#include "tileset_content_page.h"
#include "tileset_empty_page.h"
#include "tileset_info.h"
#include "ui_tileset_widget.h"

namespace tactile {

TilesetWidget::TilesetWidget(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::TilesetWidgetUI{}}

{
  m_ui->setupUi(this);

  m_contentPage = new TilesetContentPage{};
  m_emptyPage = new TilesetEmptyPage{};

  m_emptyIndex = m_ui->stackedWidget->addWidget(m_emptyPage);
  m_contentIndex = m_ui->stackedWidget->addWidget(m_contentPage);

  m_ui->stackedWidget->setCurrentIndex(m_emptyIndex);

  connect(m_contentPage,
          &TilesetContentPage::s_requested_tileset,
          this,
          &TilesetWidget::s_requested_tileset);

  connect(m_emptyPage,
          &TilesetEmptyPage::s_requested_tileset,
          this,
          &TilesetWidget::s_requested_tileset);
}

TilesetWidget::~TilesetWidget() noexcept
{
  delete m_ui;
}

void TilesetWidget::add_tileset(const TilesetInfo& info,
                                const QString& tabName) noexcept
{
  if (info.image->isNull()) {
    return;
  }

  const auto wasEmpty = m_contentPage->empty();

  m_contentPage->add_tileset(info, tabName);

  if (wasEmpty) {
    m_ui->stackedWidget->setCurrentIndex(m_contentIndex);
  }
}

void TilesetWidget::remove_tileset(int id) noexcept
{
  m_contentPage->remove_tileset(id);
}

}  // namespace tactile

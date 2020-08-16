#include "tileset_widget.hpp"

#include "tileset_content_page.hpp"
#include "tileset_empty_page.hpp"
#include "tileset_info.hpp"
#include "ui_tileset_widget.h"

using tactile::model::tileset_info;

namespace tactile::gui {

tileset_widget::tileset_widget(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::TilesetWidgetUI{}}

{
  m_ui->setupUi(this);

  m_contentPage = new tileset_content_page{};
  m_emptyPage = new tileset_empty_page{};

  m_emptyIndex = m_ui->stackedWidget->addWidget(m_emptyPage);
  m_contentIndex = m_ui->stackedWidget->addWidget(m_contentPage);

  m_ui->stackedWidget->setCurrentIndex(m_emptyIndex);

  connect(m_contentPage,
          &tileset_content_page::request_add_tileset,
          this,
          &tileset_widget::request_new_tileset);

  connect(m_emptyPage,
          &tileset_empty_page::request_new_tileset,
          this,
          &tileset_widget::request_new_tileset);
}

tileset_widget::~tileset_widget() noexcept
{
  delete m_ui;
}

void tileset_widget::add_tileset(const tileset_info& info,
                                 const QString& tabName) noexcept
{
  if (info.image.isNull()) {
    return;
  }

  const auto wasEmpty = m_contentPage->empty();

  m_contentPage->add_tileset(info, tabName);

  if (wasEmpty) {
    m_ui->stackedWidget->setCurrentIndex(m_contentIndex);
  }
}

void tileset_widget::remove_tileset(int id) noexcept
{
  m_contentPage->remove_tileset(id);
}

}  // namespace tactile::gui

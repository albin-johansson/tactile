#include "tileset_widget.hpp"

#include "tileset_content_page.hpp"
#include "tileset_empty_page.hpp"
#include "ui_tileset_widget.h"

namespace tactile::gui {

tileset_widget::tileset_widget(QWidget* parent)
    : QWidget{parent},
      m_ui{new Ui::tileset_widget{}}

{
  m_ui->setupUi(this);

  m_contentPage = new tileset_content_page{this};
  m_emptyPage = new tileset_empty_page{this};

  m_emptyIndex = m_ui->stackedWidget->addWidget(m_emptyPage);
  m_contentIndex = m_ui->stackedWidget->addWidget(m_contentPage);

  m_ui->stackedWidget->setCurrentIndex(m_emptyIndex);

  connect(m_emptyPage,
          &tileset_empty_page::request_new_tileset,
          this,
          &tileset_widget::request_new_tileset);

  connect(m_contentPage,
          &tileset_content_page::add_new_tileset,
          this,
          &tileset_widget::request_new_tileset);

  connect(m_contentPage,
          &tileset_content_page::selected_tileset,
          this,
          &tileset_widget::selected_tileset);

  connect(m_contentPage,
          &tileset_content_page::removed_tileset,
          this,
          &tileset_widget::removed_tileset);

  connect(m_contentPage,
          &tileset_content_page::tileset_selection_changed,
          this,
          &tileset_widget::tileset_selection_changed);

  connect(m_contentPage, &tileset_content_page::switch_to_empty_page, [this]() {
    m_ui->stackedWidget->setCurrentIndex(m_emptyIndex);
  });
  connect(m_contentPage,
          &tileset_content_page::switch_to_content_page,
          [this]() { m_ui->stackedWidget->setCurrentIndex(m_contentIndex); });
}

tileset_widget::~tileset_widget() noexcept
{
  delete m_ui;
}

void tileset_widget::add_tileset(map_id map,
                                 tileset_id id,
                                 const core::tileset& tileset)
{
  const auto wasEmpty = m_contentPage->empty();
  m_contentPage->add_tileset(map, id, tileset);
  if (wasEmpty) {
    m_ui->stackedWidget->setCurrentIndex(m_contentIndex);
  }
}

}  // namespace tactile::gui

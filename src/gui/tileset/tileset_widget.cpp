#include "tileset_widget.hpp"

#include "tileset_content_page.hpp"
#include "tileset_empty_page.hpp"
#include "ui_tileset_widget.h"

namespace tactile::gui {

tileset_widget::tileset_widget(QWidget* parent)
    : QWidget{parent}
    , m_ui{new Ui::tileset_widget{}}
{
  m_ui->setupUi(this);

  m_contentPage = new tileset_content_page{this};
  m_emptyPage = new tileset_empty_page{this};

  m_emptyIndex = m_ui->stackedWidget->addWidget(m_emptyPage);
  m_contentIndex = m_ui->stackedWidget->addWidget(m_contentPage);
  m_ui->stackedWidget->setCurrentIndex(m_emptyIndex);

  // clang-format off
  connect(m_emptyPage, &tileset_empty_page::ui_add_tileset, this, &tileset_widget::ui_add_tileset);
  connect(m_contentPage, &tileset_content_page::ui_add_tileset, this, &tileset_widget::ui_add_tileset);
  connect(m_contentPage, &tileset_content_page::ui_select_tileset, this, &tileset_widget::ui_select_tileset);
  connect(m_contentPage, &tileset_content_page::ui_remove_tileset, this, &tileset_widget::ui_remove_tileset);
  connect(m_contentPage, &tileset_content_page::ui_rename_tileset, this, &tileset_widget::ui_rename_tileset);
  connect(m_contentPage, &tileset_content_page::ui_set_tileset_selection, this, &tileset_widget::ui_set_tileset_selection);
  // clang-format on

  connect(m_contentPage, &tileset_content_page::switch_to_empty_page, [this] {
    m_ui->stackedWidget->setCurrentIndex(m_emptyIndex);
  });
  connect(m_contentPage, &tileset_content_page::switch_to_content_page, [this] {
    m_ui->stackedWidget->setCurrentIndex(m_contentIndex);
  });
}

tileset_widget::~tileset_widget() noexcept
{
  delete m_ui;
}

void tileset_widget::added_tileset(const map_id map,
                                   const tileset_id id,
                                   const core::tileset& tileset)
{
  const auto wasEmpty = m_contentPage->is_empty();
  m_contentPage->added_tileset(map, id, tileset);
  if (wasEmpty) {
    m_ui->stackedWidget->setCurrentIndex(m_contentIndex);
  }
}

void tileset_widget::removed_tileset(const tileset_id id)
{
  m_contentPage->removed_tileset(id);
}

void tileset_widget::selected_map(const map_id id)
{
  m_contentPage->selected_map(id);
}

}  // namespace tactile::gui

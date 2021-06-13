#include "tileset_widget.hpp"

#include "init_ui.hpp"
#include "tileset_content_page.hpp"
#include "tileset_empty_page.hpp"
#include "ui_tileset_widget.h"

namespace tactile {

TilesetWidget::TilesetWidget(QWidget* parent)
    : QWidget{parent}
    , mUi{InitUi<Ui::TilesetWidget>(this)}
    , mContentPage{new TilesetContentPage{this}}
    , mEmptyPage{new TilesetEmptyPage{this}}
    , mEmptyIndex{mUi->stackedWidget->addWidget(mEmptyPage)}
    , mContentIndex{mUi->stackedWidget->addWidget(mContentPage)}
{
  mUi->stackedWidget->setCurrentIndex(mEmptyIndex);

  // clang-format off
  connect(mEmptyPage, &TilesetEmptyPage::S_AddTileset, this, &TilesetWidget::S_AddTileset);
  connect(mContentPage, &TilesetContentPage::S_AddTileset, this, &TilesetWidget::S_AddTileset);
  connect(mContentPage, &TilesetContentPage::S_SelectTileset, this, &TilesetWidget::S_SelectTileset);
  connect(mContentPage, &TilesetContentPage::S_RemoveTileset, this, &TilesetWidget::S_RemoveTileset);
  connect(mContentPage, &TilesetContentPage::S_RenameTileset, this, &TilesetWidget::S_RenameTileset);
  connect(mContentPage, &TilesetContentPage::S_ShowProperties, this, &TilesetWidget::S_ShowProperties);
  connect(mContentPage, &TilesetContentPage::S_SetTilesetSelection, this, &TilesetWidget::S_SetTilesetSelection);
  connect(mContentPage, &TilesetContentPage::S_SwitchToEmptyPage, this, &TilesetWidget::OnSwitchToEmptyPage);
  connect(mContentPage, &TilesetContentPage::S_SwitchToContentPage, this, &TilesetWidget::OnSwitchToContentPage);
  // clang-format on
}

TilesetWidget::~TilesetWidget() noexcept = default;

void TilesetWidget::OnAddedTileset(const map_id map,
                                   const tileset_id id,
                                   const core::Tileset& tileset)
{
  const auto wasEmpty = mContentPage->IsEmpty();
  mContentPage->OnAddedTileset(map, id, tileset);
  if (wasEmpty)
  {
    mUi->stackedWidget->setCurrentIndex(mContentIndex);
  }
}

void TilesetWidget::OnRemovedTileset(const tileset_id id)
{
  mContentPage->OnRemovedTileset(id);
}

void TilesetWidget::OnRenamedTileset(const tileset_id id, const QString& name)
{
  mContentPage->OnRenamedTileset(id, name);
}

void TilesetWidget::OnSwitchedMap(const map_id id)
{
  mContentPage->OnSelectedMap(id);
}

void TilesetWidget::OnSwitchToEmptyPage()
{
  mUi->stackedWidget->setCurrentIndex(mEmptyIndex);
}

void TilesetWidget::OnSwitchToContentPage()
{
  mUi->stackedWidget->setCurrentIndex(mContentIndex);
}

}  // namespace tactile

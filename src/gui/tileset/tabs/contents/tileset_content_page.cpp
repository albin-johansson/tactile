#include "tileset_content_page.hpp"

#include <QLineEdit>    // QLineEdit
#include <QPushButton>  // QPushButton
#include <QTabBar>      // QTabBar

#include "icons.hpp"
#include "init_ui.hpp"
#include "tab_widget.hpp"
#include "tileset.hpp"
#include "tileset_tab.hpp"
#include "tileset_tab_context_menu.hpp"
#include "tileset_tab_manager.hpp"
#include "ui_tileset_content_page.h"

namespace tactile {

TilesetContentPage::TilesetContentPage(QWidget* parent)
    : QWidget{parent}
    , mUi{InitUi<Ui::TilesetContentPage>(this)}
    , mTabWidget{new TabWidget{this}}
    , mContextMenu{new TilesetTabContextMenu{this}}
{
  mTabWidget->setTabsClosable(false);
  mTabWidget->setContextMenuPolicy(Qt::CustomContextMenu);
  AddCornerButton();

  mUi->gridLayout->addWidget(mTabWidget);

  // clang-format off
  connect(mTabWidget, &TabWidget::S_EditedTab, this, &TilesetContentPage::OnEditedTab);
  connect(mTabWidget, &QTabWidget::customContextMenuRequested, this, &TilesetContentPage::OnContextMenuRequested);
  connect(mTabWidget, &QTabWidget::tabCloseRequested, this, &TilesetContentPage::OnTabCloseRequested);
  connect(mTabWidget, &QTabWidget::tabBarClicked, this, &TilesetContentPage::OnTabBarClicked);
  connect(mTabWidget, &QTabWidget::currentChanged, this, &TilesetContentPage::OnCurrentTabChanged);

  connect(mContextMenu, &TilesetTabContextMenu::S_Rename, this, &TilesetContentPage::OnContextActionRename);
  connect(mContextMenu, &TilesetTabContextMenu::S_Remove, this, &TilesetContentPage::OnContextActionRemove);
  connect(mContextMenu, &TilesetTabContextMenu::S_ShowProperties, this, &TilesetContentPage::OnContextActionShowProperties);
  // clang-format on
}

TilesetContentPage::~TilesetContentPage() noexcept = default;

void TilesetContentPage::AddCornerButton()
{
  auto* button = new QPushButton{mTabWidget};
  button->setIcon(IconAdd());

  connect(button,
          &QPushButton::pressed,
          this,
          &TilesetContentPage::S_AddTileset);

  mTabWidget->setCornerWidget(button, Qt::Corner::TopRightCorner);
}

void TilesetContentPage::OnSelectedMap(const map_id map)
{
  {
    QSignalBlocker blocker{mTabWidget};  // avoid `S_SelectTileset`

    if (mCurrentMap)
    {
      mTabWidget->clear();
    }

    SwitchTo(map);
  }

  if (CurrentManager().IsEmpty())
  {
    emit S_SwitchToEmptyPage();
  }
  else
  {
    emit S_SwitchToContentPage();
  }
}

void TilesetContentPage::OnAddedTileset(const map_id map,
                                        const tileset_id id,
                                        const core::Tileset& tileset)
{
  Q_ASSERT(!CurrentManager().Contains(id));

  auto* tab = new TilesetTab{id, tileset, mTabWidget};

  // clang-format off
  connect(tab, &TilesetTab::S_SetTilesetSelection, this, &TilesetContentPage::S_SetTilesetSelection);
  // clang-format on

  mTabManagers.at(map).Add(id, tab);

  const auto index = mTabWidget->addTab(tab, tileset.Name());
  mTabWidget->setCurrentIndex(index);
}

void TilesetContentPage::OnRemovedTileset(const tileset_id id)
{
  auto& manager = CurrentManager();
  Q_ASSERT(manager.Contains(id));

  mTabWidget->removeTab(manager.IndexOf(id));
  manager.Remove(id);
}

void TilesetContentPage::OnRenamedTileset(const tileset_id id,
                                          const QString& name)
{
  auto& manager = CurrentManager();
  Q_ASSERT(manager.Contains(id));

  mTabWidget->tabBar()->setTabText(manager.IndexOf(id), name);
}

auto TilesetContentPage::IsEmpty() const -> bool
{
  return mTabWidget->count() == 0;
}

void TilesetContentPage::SwitchTo(const map_id map)
{
  mCurrentMap = map;
  const auto& manager = mTabManagers[*mCurrentMap];

  for (const auto& [id, tab] : manager)
  {
    mTabWidget->addTab(tab, tab->Name());
  }

  mTabWidget->setCurrentIndex(manager.CachedIndex().value_or(0));
}

auto TilesetContentPage::TabFromIndex(const int index) -> TilesetTab*
{
  return qobject_cast<TilesetTab*>(mTabWidget->widget(index));
}

auto TilesetContentPage::CurrentManager() -> TilesetTabManager&
{
  return mTabManagers.at(mCurrentMap.value());
}

void TilesetContentPage::OnContextMenuRequested(const QPoint& pos)
{
  const auto index = mTabWidget->tabBar()->tabAt(pos);
  if (index != -1)
  {
    mContextMenu->SetTabIndex(index);
    mContextMenu->exec(mapToGlobal(pos));
  }
}

void TilesetContentPage::OnContextActionRename(const int index)
{
  mTabWidget->EditTab(index);
}

void TilesetContentPage::OnContextActionRemove(const int index)
{
  if (auto* tab = TabFromIndex(index))
  {
    emit S_RemoveTileset(tab->Id());
  }
}

void TilesetContentPage::OnContextActionShowProperties(const int index)
{
  if (auto* tab = TabFromIndex(index))
  {
    emit S_ShowProperties(tab->Id());
  }
}

void TilesetContentPage::OnEditedTab(const int index)
{
  if (auto* tab = TabFromIndex(index))
  {
    emit S_RenameTileset(tab->Id(), mTabWidget->tabBar()->tabText(index));
  }
}

void TilesetContentPage::OnTabCloseRequested(const int index)
{
  if (auto* tab = TabFromIndex(index))
  {
    emit S_RemoveTileset(tab->Id());
  }
}

void TilesetContentPage::OnTabBarClicked(const int index)
{
  if (auto* tab = TabFromIndex(index))
  {
    emit S_SelectTileset(tab->Id());
  }
}

void TilesetContentPage::OnCurrentTabChanged(const int index)
{
  if (index != -1)
  {
    CurrentManager().SetCachedIndex(index);
    if (auto* item = TabFromIndex(index))
    {
      emit S_SelectTileset(item->Id());
    }
  }
  else
  {
    emit S_SwitchToEmptyPage();
  }
}

}  // namespace tactile

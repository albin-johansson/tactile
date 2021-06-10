#include "tileset_content_page.hpp"

#include <QLineEdit>    // QLineEdit
#include <QPushButton>  // QPushButton
#include <QTabBar>      // QTabBar

#include "icons.hpp"
#include "init_ui.hpp"
#include "tab_widget.hpp"
#include "tileset_tab.hpp"
#include "tileset_tab_context_menu.hpp"
#include "ui_tileset_content_page.h"

namespace tactile {

TilesetContentPage::TilesetContentPage(QWidget* parent)
    : QWidget{parent}
    , mUi{init_ui<Ui::TilesetContentPage>(this)}
    , mTabWidget{new TabWidget{this}}
    , mContextMenu{new TilesetTabContextMenu{this}}
{
  mTabWidget->setTabsClosable(false);
  mTabWidget->setContextMenuPolicy(Qt::CustomContextMenu);
  AddCornerButton();

  mUi->gridLayout->addWidget(mTabWidget);

  connect(mTabWidget,
          &QTabWidget::customContextMenuRequested,
          this,
          &TilesetContentPage::TriggerContextMenu);

  connect(mContextMenu, &TilesetTabContextMenu::S_Rename, [this](int index) {
    mTabWidget->EditTab(index);
  });

  connect(mContextMenu, &TilesetTabContextMenu::S_Remove, [this](int index) {
    if (auto* tab = TabFromIndex(index))
    {
      emit S_RemoveTileset(tab->Id());
    }
  });

  connect(mTabWidget, &TabWidget::S_EditedTab, [this](int index) {
    if (auto* tab = TabFromIndex(index))
    {
      emit S_RenameTileset(tab->Id(), mTabWidget->tabBar()->tabText(index));
    }
  });

  connect(mTabWidget, &QTabWidget::tabCloseRequested, [this](int index) {
    if (auto* tab = TabFromIndex(index))
    {
      emit S_RemoveTileset(tab->Id());
    }
  });

  connect(mTabWidget, &QTabWidget::tabBarClicked, [this](int index) {
    if (auto* tab = TabFromIndex(index))
    {
      emit S_SelectTileset(tab->Id());
    }
  });

  connect(mTabWidget, &QTabWidget::currentChanged, [this](int index) {
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
  });
}

TilesetContentPage::~TilesetContentPage() noexcept = default;

void TilesetContentPage::AddCornerButton()
{
  auto* button = new QPushButton{mTabWidget};
  button->setIcon(icons::add());

  connect(button,
          &QPushButton::pressed,
          this,
          &TilesetContentPage::S_AddTileset);

  mTabWidget->setCornerWidget(button, Qt::Corner::TopRightCorner);
}

void TilesetContentPage::TriggerContextMenu(const QPoint& pos)
{
  const auto index = mTabWidget->tabBar()->tabAt(pos);
  if (index != -1)
  {
    mContextMenu->SetTabIndex(index);
    mContextMenu->exec(mapToGlobal(pos));
  }
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
                                        const core::tileset& tileset)
{
  Q_ASSERT(!CurrentManager().Contains(id));

  auto* tab = new TilesetTab{id, tileset, mTabWidget};
  connect(tab,
          &TilesetTab::S_SetTilesetSelection,
          this,
          &TilesetContentPage::S_SetTilesetSelection);
  mTabManagers.at(map).Add(id, tab);

  const auto index = mTabWidget->addTab(tab, tileset.name());
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

}  // namespace tactile

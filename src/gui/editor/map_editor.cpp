#include "map_editor.hpp"

#include <QMouseEvent>  // QMouseEvent

#include "init_ui.hpp"
#include "map_document.hpp"
#include "map_editor_context_menu.hpp"
#include "map_tab_widget.hpp"
#include "startup_widget.hpp"
#include "ui_map_editor.h"

namespace tactile {

MapEditor::MapEditor(QWidget* parent)
    : QWidget{parent}
    , mUi{InitUi<Ui::MapEditor>(this)}
    , mTabWidget{new MapTabWidget{this}}
    , mContextMenu{new MapEditorContextMenu{this}}
    , mStartupID{mUi->stackedWidget->addWidget(new StartupWidget{this})}
    , mEditorID{mUi->stackedWidget->addWidget(mTabWidget)}
{
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setContentsMargins(0, 0, 0, 0);

  InitConnections();
}

MapEditor::~MapEditor() noexcept = default;

void MapEditor::InitConnections()
{
  // clang-format off
  connect(this, &MapEditor::S_ThemeChanged, mTabWidget, &MapTabWidget::OnThemeChanged);
  connect(mContextMenu, &MapEditorContextMenu::S_ShowMapProperties, this, &MapEditor::OnShowMapProperties);
  connect(mTabWidget, &MapTabWidget::S_RemoveMap, this, &MapEditor::S_RemoveMap);
  connect(mTabWidget, &MapTabWidget::S_ZoomIn, this, &MapEditor::S_ZoomIn);
  connect(mTabWidget, &MapTabWidget::S_ZoomOut, this, &MapEditor::S_ZoomOut);
  connect(mTabWidget, &MapTabWidget::S_MousePressed, this, &MapEditor::S_MousePressed);
  connect(mTabWidget, &MapTabWidget::S_MouseMoved, this, &MapEditor::S_MouseMoved);
  connect(mTabWidget, &MapTabWidget::S_MouseReleased, this, &MapEditor::S_MouseReleased);
  connect(mTabWidget, &MapTabWidget::S_MouseEntered, this, &MapEditor::S_MouseEntered);
  connect(mTabWidget, &MapTabWidget::S_MouseExited, this, &MapEditor::S_MouseExited);
  connect(mTabWidget, &MapTabWidget::S_SpawnContextMenu, this, &MapEditor::OnSpawnContextMenu);
  connect(mTabWidget, &MapTabWidget::currentChanged, this, &MapEditor::OnTabChanged);
  // clang-format on
}

void MapEditor::EnableStampPreview(const core::MapPosition& position)
{
  mTabWidget->EnableStampPreview(position);
}

void MapEditor::DisableStampPreview()
{
  mTabWidget->DisableStampPreview();
}

void MapEditor::AddMapTab(core::MapDocument* map,
                          const map_id id,
                          const QString& title)
{
  mTabWidget->AddTab(map, id, title);
}

void MapEditor::SelectTab(const map_id id)
{
  mTabWidget->SelectTab(id);
}

void MapEditor::CloseTab(const map_id id)
{
  mTabWidget->RemoveTab(id);
}

void MapEditor::CenterViewport()
{
  mTabWidget->CenterViewport();
}

void MapEditor::MoveViewport(const int dx, const int dy)
{
  mTabWidget->MoveViewport(dx, dy);
}

void MapEditor::ForceRedraw()
{
  mTabWidget->ForceRedraw();
}

void MapEditor::SetOpenGlEnabled(const bool enabled)
{
  mTabWidget->SetOpenGlEnabled(enabled);
}

void MapEditor::EnableStartupView()
{
  mUi->stackedWidget->setCurrentIndex(mStartupID);
}

void MapEditor::EnableEditorView()
{
  mUi->stackedWidget->setCurrentIndex(mEditorID);
}

void MapEditor::SetActiveTabName(const QString& name)
{
  mTabWidget->SetActiveTabName(name);
}

auto MapEditor::InEditorMode() const -> bool
{
  return mUi->stackedWidget->currentIndex() == mEditorID;
}

auto MapEditor::ActiveTabID() const -> Maybe<map_id>
{
  return mTabWidget->ActiveTabId();
}

auto MapEditor::ActiveTabName() const -> Maybe<QString>
{
  return mTabWidget->ActiveTabName();
}

auto MapEditor::TabCount() const -> int
{
  return mTabWidget->count();
}

void MapEditor::OnTabChanged(const int index)
{
  if (const auto id = mTabWidget->IdFromIndex(index); id)
  {
    emit S_SelectMap(*id);
  }
}

void MapEditor::OnSpawnContextMenu(const QPoint& pos)
{
  mContextMenu->exec(pos);
}

void MapEditor::OnShowMapProperties()
{
  mTabWidget->ShowMapProperties();
}

}  // namespace tactile

#include "map_tab_widget.hpp"

#include "map_document.hpp"

namespace tactile {

MapTabWidget::MapTabWidget(QWidget* parent) : TabWidget{parent}
{
  setObjectName(QStringLiteral(u"MapTabWidget"));
  setTabsClosable(true);

  // clang-format off
  connect(this, &QTabWidget::tabCloseRequested,
          this, &MapTabWidget::OnTabCloseRequested);
  // clang-format on
}

MapTabWidget::~MapTabWidget() noexcept = default;

void MapTabWidget::OnTabCloseRequested(const int index)
{
  emit S_RemoveMap(GetView(index)->Id());
  removeTab(index);
}

void MapTabWidget::OnThemeChanged()
{
  ApplyStylesheet();
}

void MapTabWidget::ForceRedraw()
{
  if (auto* view = CurrentView())
  {
    view->ForceRedraw();
  }
}

void MapTabWidget::AddTab(core::MapDocument* map,
                          map_id id,
                          const QString& title)
{
  auto* view = new MapView{map, id, this};

  // clang-format off
  connect(view, &MapView::S_MousePressed, this, &MapTabWidget::S_MousePressed);
  connect(view, &MapView::S_MouseMoved, this, &MapTabWidget::S_MouseMoved);
  connect(view, &MapView::S_MouseReleased, this, &MapTabWidget::S_MouseReleased);
  connect(view, &MapView::S_MouseEntered, this, &MapTabWidget::S_MouseEntered);
  connect(view, &MapView::S_MouseExited, this, &MapTabWidget::S_MouseExited);
  connect(view, &MapView::S_ZoomIn, this, &MapTabWidget::S_ZoomIn);
  connect(view, &MapView::S_ZoomOut, this, &MapTabWidget::S_ZoomOut);
  connect(view, &MapView::S_SpawnContextMenu, this, &MapTabWidget::S_SpawnContextMenu);
  // clang-format on

  if (title == QStringLiteral(u"map"))
  {
    addTab(view, title + QString::number(id.get()));
  }
  else
  {
    addTab(view, title);
  }
}

void MapTabWidget::RemoveTab(map_id id)
{
  if (auto* view = ViewFromId(id))
  {
    removeTab(indexOf(view));
  }
}

void MapTabWidget::SelectTab(map_id id)
{
  if (auto* view = ViewFromId(id))
  {
    setCurrentWidget(view);
  }
}

void MapTabWidget::CenterViewport()
{
  if (auto* view = CurrentView())
  {
    view->CenterViewport();
  }
}

void MapTabWidget::MoveViewport(const int dx, const int dy)
{
  if (auto* view = CurrentView())
  {
    view->MoveViewport(dx, dy);
  }
}

void MapTabWidget::EnableStampPreview(const core::MapPosition& position)
{
  if (auto* view = CurrentView())
  {
    view->EnableStampPreview(position);
  }
}

void MapTabWidget::DisableStampPreview()
{
  if (auto* view = CurrentView())
  {
    view->DisableStampPreview();
  }
}

void MapTabWidget::ShowMapProperties()
{
  if (auto* view = CurrentView())
  {
    view->ShowMapProperties();
  }
}

void MapTabWidget::SetActiveTabName(const QString& name)
{
  if (const auto index = currentIndex(); index != -1)
  {
    setTabText(index, name);
  }
}

void MapTabWidget::SetOpenGlEnabled(const bool enabled)
{
  if (auto* view = CurrentView())
  {
    view->SetOpenGlEnabled(enabled);
  }
}

auto MapTabWidget::ActiveTabId() const -> Maybe<map_id>
{
  return IdFromIndex(currentIndex());
}

auto MapTabWidget::IdFromIndex(const int index) const -> Maybe<map_id>
{
  if (const auto* view = GetView(index))
  {
    return view->Id();
  }
  else
  {
    return nothing;
  }
}

auto MapTabWidget::ActiveTabName() const -> Maybe<QString>
{
  const auto index = currentIndex();
  if (index != -1)
  {
    return tabText(index);
  }
  else
  {
    return nothing;
  }
}

auto MapTabWidget::CurrentView() -> MapView*
{
  return GetView(currentIndex());
}

auto MapTabWidget::GetView(const int index) -> MapView*
{
  return qobject_cast<MapView*>(widget(index));
}

auto MapTabWidget::GetView(const int index) const -> const MapView*
{
  return qobject_cast<MapView*>(widget(index));
}

auto MapTabWidget::ViewFromId(map_id id) -> MapView*
{
  const auto amount = count();
  for (auto i = 0; i < amount; ++i)
  {
    if (auto* pane = GetView(i); pane && pane->Id() == id)
    {
      return pane;
    }
  }
  return nullptr;
}

}  // namespace tactile

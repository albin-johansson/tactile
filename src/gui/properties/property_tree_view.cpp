#include "property_tree_view.hpp"

#include <QHeaderView>    // QHeaderView
#include <QMouseEvent>    // QMouseEvent
#include <QStandardItem>  // QStandardItem

#include "color_preview_button.hpp"
#include "file_value_widget.hpp"
#include "icons.hpp"
#include "item_model_utils.hpp"
#include "property_item_type.hpp"
#include "property_items.hpp"
#include "property_model.hpp"
#include "select_file_dialog.hpp"
#include "tactile_qstring.hpp"

namespace tactile::gui {

property_tree_view::property_tree_view(QWidget* parent) : QTreeView{parent}
{
  setObjectName(TACTILE_QSTRING(u"property_tree_view"));

  setSelectionBehavior(SelectRows);
  setSelectionMode(SingleSelection);
  setAlternatingRowColors(true);
  setUniformRowHeights(true);
  setRootIsDecorated(false);
  setIndentation(6);

  {
    auto* h = header();
    h->setSectionResizeMode(QHeaderView::ResizeToContents);
    h->setHidden(true);
    h->setSectionsMovable(false);
  }

  // clang-format off
  connect(this, &property_tree_view::expanded,
          this, &property_tree_view::when_item_expanded);

  connect(this, &property_tree_view::collapsed,
          this, &property_tree_view::when_item_collapsed);
  // clang-format on
}

void property_tree_view::restore_item_widgets()
{
  vm::visit_items(get_model(), 1, [this](QStandardItem* item) {
    const auto itemType = static_cast<vm::property_item_type>(item->type());

    if (itemType == vm::property_item_type::color)
    {
      when_color_added(item->index());
    }
    else if (itemType == vm::property_item_type::file)
    {
      when_file_added(item->index());
    }
  });
}

void property_tree_view::when_color_added(const QModelIndex& valueIndex)
{
  const auto color =
      valueIndex.data(vm::property_item_role::color).value<QColor>();
  Q_ASSERT(color.isValid());

  auto* button = new color_preview_button{color};

  const auto id = new_widget_id();
  m_widgetItems.emplace(id, get_model()->itemFromIndex(valueIndex));

  connect(button,
          &color_preview_button::color_changed,
          [this, id](const QColor& color) {
            auto* item = m_widgetItems.at(id);
            item->setData(color, vm::property_item_role::color);
          });

  setIndexWidget(valueIndex, button);
}

void property_tree_view::when_file_added(const QModelIndex& valueIndex)
{
  auto* widget = new file_value_widget{};
  widget->set_path(
      valueIndex.data(vm::property_item_role::path).value<QString>());

  const auto id = new_widget_id();
  m_widgetItems.emplace(id, get_model()->itemFromIndex(valueIndex));

  connect(widget, &file_value_widget::spawn_dialog, [this, id] {
    select_file_dialog::spawn([this, id](const QString& path) {
      auto* item = m_widgetItems.at(id);
      item->setData(path, vm::property_item_role::path);

      auto* itemWidget = indexWidget(item->index());
      if (auto* widget = qobject_cast<file_value_widget*>(itemWidget))
      {
        widget->set_path(path);
      }
    });
  });

  setIndexWidget(valueIndex, widget);
}

void property_tree_view::when_changed_type(const QModelIndex& valueIndex,
                                           const core::property_type type)
{
  if (type == core::property_type::file)
  {
    when_file_added(valueIndex);
  }
  else if (type == core::property_type::color)
  {
    when_color_added(valueIndex);
  }

  setCurrentIndex(valueIndex);
}

void property_tree_view::when_file_updated(const QModelIndex& index)
{
  if (auto* widget = qobject_cast<file_value_widget*>(indexWidget(index)))
  {
    widget->set_path(index.data(vm::property_item_role::path).value<QString>());
  }
}

void property_tree_view::when_color_updated(const QModelIndex& index)
{
  if (auto* button = qobject_cast<color_preview_button*>(indexWidget(index)))
  {
    button->set_color(
        index.data(vm::property_item_role::color).value<QColor>());
  }
}

void property_tree_view::selectionChanged(const QItemSelection& selected,
                                          const QItemSelection& deselected)
{
  QTreeView::selectionChanged(selected, deselected);

  Q_ASSERT(selected.size() == 1 || selected.empty());

  maybe<QModelIndex> selectedIndex;
  for (const auto index : selected.indexes())
  {
    selectedIndex = index;
    if (auto* widget = qobject_cast<file_value_widget*>(indexWidget(index)))
    {
      widget->enter_active_mode();
    }
  }

  for (const auto index : deselected.indexes())
  {
    if (auto* widget = qobject_cast<file_value_widget*>(indexWidget(index)))
    {
      widget->enter_idle_mode();
    }
  }

  emit selection_changed(selectedIndex);
}

void property_tree_view::rowsAboutToBeRemoved(const QModelIndex& parent,
                                              const int start,
                                              const int end)
{
  QTreeView::rowsAboutToBeRemoved(parent, start, end);

  // Yes, this range is inclusive: [start, end]
  for (auto row = start; row <= end; ++row)
  {
    if (const auto* item = get_model()->item(row, 1))
    {
      if (auto* widget = indexWidget(item->index()))
      {
        widget->hide();
      }
    }
  }
}

void property_tree_view::mousePressEvent(QMouseEvent* event)
{
  // This is done to be able to deselect properties by pressing outside of items
  selectionModel()->clear();
  QTreeView::mousePressEvent(event);

  if (event->button() == Qt::RightButton)
  {
    emit spawn_context_menu(mapToGlobal(event->pos()));
  }
}

auto property_tree_view::get_model() -> vm::property_model*
{
  auto* ptr = qobject_cast<vm::property_model*>(model());
  Q_ASSERT(ptr);
  return ptr;
}

auto property_tree_view::get_model() const -> const vm::property_model*
{
  const auto* ptr = qobject_cast<const vm::property_model*>(model());
  Q_ASSERT(ptr);
  return ptr;
}

auto property_tree_view::new_widget_id() noexcept -> int
{
  const auto next = m_nextWidgetId;
  ++m_nextWidgetId;
  return next;
}

void property_tree_view::when_item_expanded(const QModelIndex& index)
{
  if (!index.parent().isValid())
  {
    get_model()->itemFromIndex(index)->setIcon(icons::expanded());
  }
}

void property_tree_view::when_item_collapsed(const QModelIndex& index)
{
  if (!index.parent().isValid())
  {
    get_model()->itemFromIndex(index)->setIcon(icons::collapsed());
  }
}

}  // namespace tactile::gui

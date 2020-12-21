#include "property_tree_view.hpp"

#include <QDebug>
#include <QHeaderView>
#include <QShowEvent>
#include <concepts>  // invocable

#include "color_preview_button.hpp"
#include "file_value_widget.hpp"
#include "item_type.hpp"
#include "property_file_dialog.hpp"
#include "property_items.hpp"
#include "tactile_qstring.hpp"
#include "visit_items.hpp"

namespace tactile::gui {

property_tree_view::property_tree_view(QWidget* parent) : QTreeView{parent}
{
  setObjectName(TACTILE_QSTRING(u"property_tree_view"));
  setSelectionBehavior(SelectItems);
  setSelectionMode(SingleSelection);
  setAlternatingRowColors(true);
  setUniformRowHeights(true);

  setFirstColumnSpanned(0, rootIndex(), true);
  setFirstColumnSpanned(1, rootIndex(), true);

  header()->setSectionResizeMode(QHeaderView::ResizeToContents);
  header()->setHidden(true);
  header()->setSectionsMovable(false);
}

void property_tree_view::add_item_widgets()
{
  viewmodel::visit_items(get_model(), 1, [this](QStandardItem* item) {
    const auto itemType = static_cast<viewmodel::item_type>(item->type());

    if (itemType == viewmodel::item_type::color) {
      when_color_added(item->index());

    } else if (itemType == viewmodel::item_type::file) {
      when_file_added(item->index());
    }
  });
}

void property_tree_view::when_color_added(const QModelIndex& index)
{
  const auto color =
      index.data(viewmodel::property_item_role::color).value<QColor>();
  Q_ASSERT(color.isValid());

  auto* button = new color_preview_button{color};

  connect(button,
          &color_preview_button::color_changed,
          [this, index](const QColor& color) {
            Q_ASSERT(index.isValid());
            if (auto* model = get_model()) {
              if (auto* item = model->itemFromIndex(index)) {
                item->setData(color, viewmodel::property_item_role::color);
              }
            }
          });

  setIndexWidget(index, button);
}

void property_tree_view::when_file_added(const QModelIndex& index)
{
  auto* widget = new file_value_widget{};
  widget->set_path(
      index.data(viewmodel::property_item_role::path).value<QString>());

  const auto id = new_widget_id();
  m_widgetItems.emplace(id, get_model()->itemFromIndex(index));

  connect(widget, &file_value_widget::spawn_dialog, [=, this] {
    property_file_dialog::spawn([id, this](const QString& path) {
      auto* item = m_widgetItems.at(id);
      item->setData(path, viewmodel::property_item_role::path);

      if (auto* widget =
              dynamic_cast<file_value_widget*>(indexWidget(item->index()))) {
        const QFileInfo file{path};
        widget->set_path(file);
      }
    });
  });

  setIndexWidget(index, widget);
}

void property_tree_view::selectionChanged(const QItemSelection& selected,
                                          const QItemSelection& deselected)
{
  QTreeView::selectionChanged(selected, deselected);

  for (const auto index : selected.indexes()) {
    if (auto* widget =
            dynamic_cast<property_value_widget*>(indexWidget(index))) {
      widget->enter_active_mode();
    }
  }

  for (const auto index : deselected.indexes()) {
    if (auto* widget =
            dynamic_cast<property_value_widget*>(indexWidget(index))) {
      widget->enter_idle_mode();
    }
  }
}

void property_tree_view::mousePressEvent(QMouseEvent* event)
{
  QTreeView::mousePressEvent(event);
  if (!childAt(event->pos())) {
    clearSelection();
  }
}

auto property_tree_view::get_model() -> QStandardItemModel*
{
  auto* ptr = qobject_cast<QStandardItemModel*>(model());
  Q_ASSERT(ptr);
  return ptr;
}

auto property_tree_view::get_model() const -> const QStandardItemModel*
{
  const auto* ptr = qobject_cast<const QStandardItemModel*>(model());
  Q_ASSERT(ptr);
  return ptr;
}

}  // namespace tactile::gui

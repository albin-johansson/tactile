#include "property_tree_view.hpp"

#include <QDebug>
#include <QHeaderView>
#include <QMouseEvent>
#include <concepts>  // invocable

#include "color_preview_button.hpp"
#include "file_value_widget.hpp"
#include "icons.hpp"
#include "item_model_utils.hpp"
#include "item_type.hpp"
#include "property_file_dialog.hpp"
#include "property_items.hpp"
#include "property_model.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::gui {

namespace vm = viewmodel;

property_tree_view::property_tree_view(QWidget* parent) : QTreeView{parent}
{
  setObjectName(TACTILE_QSTRING(u"property_tree_view"));

  setSelectionBehavior(SelectItems);
  setSelectionMode(SingleSelection);
  setAlternatingRowColors(true);
  setUniformRowHeights(true);
  setRootIsDecorated(false);
  setIndentation(25);

  setFirstColumnSpanned(0, rootIndex(), true);
  setFirstColumnSpanned(1, rootIndex(), true);

  auto* h = header();
  h->setSectionResizeMode(QHeaderView::ResizeToContents);
  h->setHidden(true);
  h->setSectionsMovable(false);

  connect(this,
          &property_tree_view::expanded,
          [this](const QModelIndex& index) {
            if (!index.parent().isValid()) {
              get_model()->itemFromIndex(index)->setIcon(icons::expanded());
            }
          });

  connect(this,
          &property_tree_view::collapsed,
          [this](const QModelIndex& index) {
            if (!index.parent().isValid()) {
              get_model()->itemFromIndex(index)->setIcon(icons::collapsed());
            }
          });
}

void property_tree_view::setModel(QAbstractItemModel* model)
{
  QTreeView::setModel(model);
  if (auto* ptr = qobject_cast<vm::property_model*>(model); !ptr) {
    throw tactile_error{"property_tree_view must have property viewmodel!"};
  }
}

void property_tree_view::add_item_widgets()
{
  vm::visit_items(get_model(), 1, [this](QStandardItem* item) {
    const auto itemType = static_cast<vm::item_type>(item->type());

    if (itemType == vm::item_type::color) {
      when_color_added(item->index());

    } else if (itemType == vm::item_type::file) {
      when_file_added(item->index());
    }
  });
}

void property_tree_view::when_color_added(const QModelIndex& index)
{
  const auto color = index.data(vm::property_item_role::color).value<QColor>();
  Q_ASSERT(color.isValid());

  auto* button = new color_preview_button{color};

  // TODO use m_widgetItems instead of raw index capture
  connect(button,
          &color_preview_button::color_changed,
          [this, index](const QColor& color) {
            Q_ASSERT(index.isValid());
            if (auto* model = get_model()) {
              if (auto* item = model->itemFromIndex(index)) {
                item->setData(color, vm::property_item_role::color);
              }
            }
          });

  setIndexWidget(index, button);
}

void property_tree_view::when_file_added(const QModelIndex& index)
{
  auto* widget = new file_value_widget{};
  widget->set_path(index.data(vm::property_item_role::path).value<QString>());

  const auto id = new_widget_id();
  m_widgetItems.emplace(id, get_model()->itemFromIndex(index));

  connect(widget, &file_value_widget::spawn_dialog, [id, this] {
    property_file_dialog::spawn([id, this](const QString& path) {
      auto* item = m_widgetItems.at(id);
      item->setData(path, vm::property_item_role::path);

      if (auto* widget =
              qobject_cast<file_value_widget*>(indexWidget(item->index()))) {
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

  Q_ASSERT(selected.size() == 1 || selected.empty());

  for (const auto index : selected.indexes()) {
    if (auto* widget =
            qobject_cast<property_value_widget*>(indexWidget(index))) {
      widget->enter_active_mode();
    }
  }

  for (const auto index : deselected.indexes()) {
    if (auto* widget =
            qobject_cast<property_value_widget*>(indexWidget(index))) {
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

}  // namespace tactile::gui

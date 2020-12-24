#pragma once

#include <QWidget>

#include "map_document.hpp"
#include "property.hpp"
#include "property_model.hpp"
#include "maybe.hpp"

namespace Ui {
class properties_widget;
}

namespace tactile::gui {

class property_tree_view;
class property_context_menu;

class properties_widget final : public QWidget
{
  Q_OBJECT

 public:
  explicit properties_widget(QWidget* parent = nullptr);

  ~properties_widget() noexcept override;

 public slots:
  void selected_map(const core::map_document& document);

 private:
  Ui::properties_widget* m_ui{};
  property_tree_view* m_treeView{};
  property_context_menu* m_contextMenu{};
  vm::property_model* m_model{};
  maybe<QString> m_nameCopy;
  maybe<core::property> m_propertyCopy;

  [[nodiscard]] auto property_name(const QModelIndex& index) const -> QString;

  [[nodiscard]] auto current_property_name() const -> QString;

 private slots:
  void selection_changed(maybe<QModelIndex> index);

  void copy_property_requested();

  void paste_property_requested();

  void new_property_requested();

  void remove_property_requested();

  void rename_property_requested();

  void change_type_requested(core::property::type type);

  void when_double_clicked();

  void spawn_context_menu(const QPoint& pos);
};

}  // namespace tactile::gui

#pragma once

#include <QWidget>  // QWidget

#include "forward_declare.hpp"
#include "maybe.hpp"
#include "not_null.hpp"
#include "property.hpp"
#include "smart_pointers.hpp"

TACTILE_FORWARD_DECLARE(Ui, properties_widget)
TACTILE_FORWARD_DECLARE(tactile::core, map_document)
TACTILE_FORWARD_DECLARE(tactile::core, property_manager)
TACTILE_FORWARD_DECLARE(tactile::vm, property_model)
TACTILE_FORWARD_DECLARE(tactile::gui, property_tree_view)
TACTILE_FORWARD_DECLARE(tactile::gui, property_context_menu)

namespace tactile::gui {

class properties_widget final : public QWidget
{
  Q_OBJECT

 public:
  explicit properties_widget(QWidget* parent = nullptr);

  ~properties_widget() noexcept override;

 public slots:
  void show_map(not_null<core::property_manager*> manager);

  void show_layer(not_null<core::property_manager*> manager);

  void added_property(const QString& name);

  void about_to_remove_property(const QString& name);

  void updated_property(const QString& name);

  void changed_property_type(const QString& name);

  void renamed_property(const QString& oldName, const QString& newName);

 private:
  unique<Ui::properties_widget> m_ui;
  property_tree_view* m_view{};
  property_context_menu* m_contextMenu{};
  unique<vm::property_model> m_model;
  maybe<QString> m_nameCopy;
  maybe<core::property> m_propertyCopy;

  void change_model(not_null<core::property_manager*> manager);

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

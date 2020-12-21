#pragma once

#include <QWidget>

#include "map_document.hpp"
#include "property.hpp"
#include "property_viewmodel.hpp"

namespace Ui {
class properties_widget;
}

namespace tactile::gui {

class property_tree_view;

class properties_widget final : public QWidget
{
  Q_OBJECT

 public:
  explicit properties_widget(QWidget* parent = nullptr);

 public slots:
  void selected_map(const core::map_document& document);

 private:
  Ui::properties_widget* m_ui{};
  property_tree_view* m_treeView{};
  viewmodel::property_viewmodel* m_model{};

 private slots:
  void new_property_requested();
};

}  // namespace tactile::gui

#pragma once

#include <QTreeWidgetItem>

#include "algorithm.hpp"
#include "tactile_error.hpp"

namespace tactile::gui {

class property_tree_item final : public QTreeWidgetItem
{
 public:
  explicit property_tree_item(QTreeWidgetItem* parent = nullptr);

  /**
   * \brief Creates and returns a tree widget item.
   *
   * \param name the name of the tree widget item.
   * \param value the value that will be displayed in the item.
   * \param parent a pointer to the parent tree widget item.
   *
   * \since 0.2.0
   */
  property_tree_item(const QString& name,
                     const QString& value,
                     QTreeWidgetItem* parent = nullptr);

  template <arithmetic T>
  property_tree_item(const QString& name,
                     T value,
                     QTreeWidgetItem* parent = nullptr)
      : QTreeWidgetItem{parent}
  {
    const auto str = QString::number(value);
    if (str.isNull()) {
      throw tactile_error{"Could not create tree item with arithmetic value!"};
    }
    setText(0, name);
    setText(1, str);
  }

  void set_name_editable(bool editable) noexcept;

  [[nodiscard]] auto is_name_editable() const noexcept -> bool;

 private:
  bool m_nameEditable{false};
};

}  // namespace tactile::gui

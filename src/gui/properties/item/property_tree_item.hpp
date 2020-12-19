#pragma once

#include "algorithm.hpp"
#include "property.hpp"
#include "tactile_error.hpp"
#include "tree_widget_item.hpp"

namespace tactile::gui {

// TODO all items should provide two "states": "idle view" and "when focused"

/**
 * \class property_tree_item
 *
 * \brief The ABC for all property tree widget items.
 *
 * \since 0.2.0
 *
 * \headerfile property_tree_item.hpp
 */
class property_tree_item : public tree_widget_item
{
 public:
  /**
   * \brief Sets whether or not the name is editable.
   *
   * \param editable `true` if the name should be editable; `false` otherwise.
   *
   * \since 0.2.0
   */
  void set_name_editable(bool editable);

  /**
   * \brief Sets whether or not the value is editable.
   *
   * \param editable `true` if the value should be editable; `false` otherwise.
   *
   * \since 0.2.0
   */
  void set_value_editable(bool editable);

  virtual void enable_focus_view()
  {}

  virtual void enable_idle_view()
  {}

  // clang-format off

  /**
   * \brief Sets the value of the item.
   *
   * \pre the supplied property value must be of the associated type.
   *
   * \param property the new value of the item.
   *
   * \since 0.2.0
   */
  virtual void set_value(const core::property& property) = 0;

  /**
   * \brief Returns the property type associated with the item.
   *
   * \return the property type associated with the item.
   *
   * \since 0.2.0
   */
  [[nodiscard]]
  virtual auto property_type() const noexcept -> core::property::type = 0;

  /**
   * \brief Returns a pointer to the associated value widget.
   *
   * \details There might not be a value widget, in which case this function
   * returns a null pointer.
   *
   * \return a pointer to the value widget, might be null.
   *
   * \since 0.2.0
   */
  [[nodiscard]]
  virtual auto get_value_widget() -> QWidget* = 0;

  /**
   * \brief Indicates whether or not the item is an "inline" item.
   *
   * \details A property tree item is considered to be an inline item if it can
   * be edited in the widget adjacent to the name widget.
   *
   * \return `true` if the property item is an "inline" item; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]]
  virtual auto is_inline() const noexcept -> bool = 0;

  /**
   * \brief Indicates whether or not the name is editable.
   *
   * \return `true` if the name is editable; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]]
  auto is_name_editable() const noexcept -> bool;

  /**
   * \brief Indicates whether or not the value is editable.
   *
   * \return `true` if the value is editable; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]]
  auto is_value_editable() const noexcept -> bool;

  // clang-format on

 protected:
  explicit property_tree_item(QTreeWidgetItem* parent);

 private:
  bool m_isNameEditable{true};
  bool m_isValueEditable{true};
};

}  // namespace tactile::gui

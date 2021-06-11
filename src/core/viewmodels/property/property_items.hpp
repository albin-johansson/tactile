#pragma once

#include <QStandardItem>  // QStandardItem

#include "not_null.hpp"
#include "property.hpp"
#include "property_item_role.hpp"
#include "property_item_type.hpp"
#include "tactile_qstring.hpp"

namespace tactile::vm {

class root_item final : public QStandardItem
{
 public:
  explicit root_item(const QString& name) : QStandardItem{name}
  {
    setEditable(false);
    setSelectable(false);
    setColumnCount(1);
  }
};

class string_item final : public QStandardItem
{
 public:
  string_item() : QStandardItem{}
  {
    setEditable(true);
    setToolTip(TACTILE_QSTRING(u"(string)"));
  }

  [[nodiscard]] auto type() const -> int override
  {
    return static_cast<int>(PropertyItemType::String);
  }
};

class int_item final : public QStandardItem
{
 public:
  int_item() : QStandardItem{}
  {
    setToolTip(TACTILE_QSTRING(u"(int)"));
  }

  [[nodiscard]] auto type() const -> int override
  {
    return static_cast<int>(PropertyItemType::Integer);
  }
};

class float_item final : public QStandardItem
{
 public:
  float_item() : QStandardItem{}
  {
    setToolTip(TACTILE_QSTRING(u"(float)"));
  }

  [[nodiscard]] auto type() const -> int override
  {
    return static_cast<int>(PropertyItemType::Floating);
  }
};

class bool_item final : public QStandardItem
{
 public:
  bool_item() : QStandardItem{}
  {
    setEditable(false);
    setCheckable(true);
    setToolTip(TACTILE_QSTRING(u"(bool)"));
  }

  [[nodiscard]] auto type() const -> int override
  {
    return static_cast<int>(PropertyItemType::Boolean);
  }
};

class object_item final : public QStandardItem
{
 public:
  object_item() : QStandardItem{}
  {
    setEditable(false);
    setToolTip(TACTILE_QSTRING(u"(object)"));
  }

  [[nodiscard]] auto type() const -> int override
  {
    return static_cast<int>(PropertyItemType::Object);
  }
};

class color_item final : public QStandardItem
{
 public:
  color_item() : QStandardItem{}
  {
    setEditable(false);
    setToolTip(TACTILE_QSTRING(u"(color)"));
  }

  [[nodiscard]] auto type() const -> int override
  {
    return static_cast<int>(PropertyItemType::Color);
  }
};

class file_item final : public QStandardItem
{
 public:
  file_item() : QStandardItem{}
  {
    setToolTip(TACTILE_QSTRING(u"(file)"));
    setEditable(false);
  }

  [[nodiscard]] auto type() const -> int override
  {
    return static_cast<int>(PropertyItemType::File);
  }
};

/**
 * \brief Updates the data of a property item.
 *
 * \pre `item` cannot be null.
 * \pre `item` and `p` must be associated with the same property type.
 *
 * \param item a pointer to the property that will be updated.
 * \param property the property which will be used as the new item data.
 *
 * \throws TactileError if the supplied item isn't a property item.
 *
 * \since 0.2.0
 */
void update_item_data(not_null<QStandardItem*> item,
                      const core::Property& property);

/**
 * \brief Returns the property value of an item.
 *
 * \pre `item` cannot be null.
 *
 * \param item a pointer to the property item to obtain the property value of.
 *
 * \return the property value associated with an item.
 *
 * \since 0.2.0
 */
[[nodiscard]] auto item_to_property(not_null<const QStandardItem*> item)
    -> core::Property;

}  // namespace tactile::vm

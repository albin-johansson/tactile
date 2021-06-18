#pragma once

#include <QStandardItem>  // QStandardItem

#include "not_null.hpp"
#include "property.hpp"
#include "property_item_role.hpp"
#include "property_item_type.hpp"
#include "to_underlying.hpp"

namespace tactile::vm {

class RootItem final : public QStandardItem
{
 public:
  explicit RootItem(const QString& name) : QStandardItem{name}
  {
    setEditable(false);
    setSelectable(false);
    setColumnCount(1);
  }
};

class StringItem final : public QStandardItem
{
 public:
  StringItem() : QStandardItem{}
  {
    setEditable(true);
    setToolTip(QStringLiteral(u"(string)"));
  }

  [[nodiscard]] auto type() const -> int override
  {
    return ToUnderlying(PropertyItemType::String);
  }
};

class IntItem final : public QStandardItem
{
 public:
  IntItem() : QStandardItem{}
  {
    setToolTip(QStringLiteral(u"(int)"));
  }

  [[nodiscard]] auto type() const -> int override
  {
    return ToUnderlying(PropertyItemType::Integer);
  }
};

class FloatItem final : public QStandardItem
{
 public:
  FloatItem() : QStandardItem{}
  {
    setToolTip(QStringLiteral(u"(float)"));
  }

  [[nodiscard]] auto type() const -> int override
  {
    return ToUnderlying(PropertyItemType::Floating);
  }
};

class BoolItem final : public QStandardItem
{
 public:
  BoolItem() : QStandardItem{}
  {
    setEditable(false);
    setCheckable(true);
    setToolTip(QStringLiteral(u"(bool)"));
  }

  [[nodiscard]] auto type() const -> int override
  {
    return ToUnderlying(PropertyItemType::Boolean);
  }
};

class ObjectItem final : public QStandardItem
{
 public:
  ObjectItem() : QStandardItem{}
  {
    setEditable(false);
    setToolTip(QStringLiteral(u"(object)"));
  }

  [[nodiscard]] auto type() const -> int override
  {
    return ToUnderlying(PropertyItemType::Object);
  }
};

class ColorItem final : public QStandardItem
{
 public:
  ColorItem() : QStandardItem{}
  {
    setEditable(false);
    setToolTip(QStringLiteral(u"(color)"));
  }

  [[nodiscard]] auto type() const -> int override
  {
    return ToUnderlying(PropertyItemType::Color);
  }
};

class FileItem final : public QStandardItem
{
 public:
  FileItem() : QStandardItem{}
  {
    setToolTip(QStringLiteral(u"(file)"));
    setEditable(false);
  }

  [[nodiscard]] auto type() const -> int override
  {
    return ToUnderlying(PropertyItemType::File);
  }
};

/**
 * \brief Updates the data of a property item.
 *
 * \pre `item` cannot be null.
 * \pre `item` and `property` must be associated with the same property type.
 *
 * \param item a pointer to the property that will be updated.
 * \param property the property which will be used as the new item data.
 *
 * \throws TactileError if the supplied item isn't a property item.
 *
 * \since 0.2.0
 */
void UpdateItemData(not_null<QStandardItem*> item,
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
[[nodiscard]] auto ItemToProperty(not_null<const QStandardItem*> item)
    -> core::Property;

}  // namespace tactile::vm

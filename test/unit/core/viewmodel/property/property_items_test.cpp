#include "property_items.hpp"

#include <gtest/gtest.h>

#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

using namespace tactile;

TEST(PropertyItems, UpdateItemData)
{
  {  // Integer item
    const core::property property{123};

    vm::int_item item;
    EXPECT_NO_THROW(vm::update_item_data(&item, property));

    ASSERT_TRUE(item.data(Qt::EditRole).canConvert<int>());
    EXPECT_EQ(property.AsInteger(), item.data(Qt::EditRole).value<int>());
  }

  {  // Floating item
    const core::property property{12.3};

    vm::float_item item;
    EXPECT_NO_THROW(vm::update_item_data(&item, property));

    ASSERT_TRUE(item.data(Qt::EditRole).canConvert<double>());
    EXPECT_EQ(property.AsFloating(), item.data(Qt::EditRole).value<double>());
  }

  {  // Boolean item
    const core::property property{true};

    vm::bool_item item;
    EXPECT_NO_THROW(vm::update_item_data(&item, property));

    ASSERT_TRUE(item.data(Qt::CheckStateRole).canConvert<bool>());
    EXPECT_EQ(property.AsBoolean(),
              item.data(Qt::CheckStateRole).value<bool>());
  }

  {  // String item
    const core::property property{TACTILE_QSTRING(u"foo")};

    vm::string_item item;
    EXPECT_NO_THROW(vm::update_item_data(&item, property));

    ASSERT_TRUE(item.data(Qt::EditRole).canConvert<QString>());
    EXPECT_EQ(property.AsString(), item.data(Qt::EditRole).value<QString>());
  }

  {  // Color item
    const core::property property{QColor{Qt::cyan}};

    vm::color_item item;
    EXPECT_NO_THROW(vm::update_item_data(&item, property));

    const auto role = vm::property_item_role::color;
    ASSERT_TRUE(item.data(role).canConvert<QColor>());
    EXPECT_EQ(property.AsColor(), item.data(role).value<QColor>());
  }

  {  // File item
    const core::property property{QFileInfo{TACTILE_QSTRING("foo/bar")}};

    vm::file_item item;
    EXPECT_NO_THROW(vm::update_item_data(&item, property));

    const auto role = vm::property_item_role::path;
    ASSERT_TRUE(item.data(role).canConvert<QString>());
    EXPECT_EQ(property.AsFile().absoluteFilePath(), item.data(role).value<QString>());
  }

  // TODO object item
}

TEST(PropertyItems, ItemToProperty)
{
  {  // Integer item
    vm::int_item item;
    item.setData(123, Qt::EditRole);

    const auto property = vm::item_to_property(&item);
    ASSERT_TRUE(property.IsInteger());
    EXPECT_EQ(123, property.AsInteger());
  }

  {  // Floating item
    vm::float_item item;
    item.setData(12.3, Qt::EditRole);

    const auto property = vm::item_to_property(&item);
    ASSERT_TRUE(property.IsFloating());
    EXPECT_EQ(12.3, property.AsFloating());
  }

  {  // Boolean item
    vm::bool_item item;
    item.setData(Qt::Checked, Qt::CheckStateRole);

    const auto property = vm::item_to_property(&item);
    ASSERT_TRUE(property.IsBoolean());
    EXPECT_TRUE(property.AsBoolean());
  }

  {  // String item
    vm::string_item item;
    item.setData(TACTILE_QSTRING(u"foo"), Qt::EditRole);

    const auto property = vm::item_to_property(&item);
    ASSERT_TRUE(property.IsString());
    EXPECT_EQ(TACTILE_QSTRING(u"foo"), property.AsString());
  }

  {  // Color item
    vm::color_item item;
    item.setData(QColor{Qt::magenta}, vm::property_item_role::color);

    const auto property = vm::item_to_property(&item);
    ASSERT_TRUE(property.IsColor());
    EXPECT_EQ(QColor{Qt::magenta}, property.AsColor());
  }

  {  // File item
    vm::file_item item;

    const auto file = TACTILE_QSTRING(u"foo/bar");
    item.setData(file, vm::property_item_role::path);

    const auto property = vm::item_to_property(&item);
    ASSERT_TRUE(property.IsFile());
    EXPECT_EQ(QFileInfo{file}.path(), property.AsFile().path());
  }

  // TODO object item
}

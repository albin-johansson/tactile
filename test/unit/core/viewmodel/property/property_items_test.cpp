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
    EXPECT_EQ(property.as_integer(), item.data(Qt::EditRole).value<int>());
  }

  {  // Floating item
    const core::property property{12.3};

    vm::float_item item;
    EXPECT_NO_THROW(vm::update_item_data(&item, property));

    ASSERT_TRUE(item.data(Qt::EditRole).canConvert<double>());
    EXPECT_EQ(property.as_floating(), item.data(Qt::EditRole).value<double>());
  }

  {  // Boolean item
    const core::property property{true};

    vm::bool_item item;
    EXPECT_NO_THROW(vm::update_item_data(&item, property));

    ASSERT_TRUE(item.data(Qt::CheckStateRole).canConvert<bool>());
    EXPECT_EQ(property.as_boolean(),
              item.data(Qt::CheckStateRole).value<bool>());
  }

  {  // String item
    const core::property property{TACTILE_QSTRING(u"foo")};

    vm::string_item item;
    EXPECT_NO_THROW(vm::update_item_data(&item, property));

    ASSERT_TRUE(item.data(Qt::EditRole).canConvert<QString>());
    EXPECT_EQ(property.as_string(), item.data(Qt::EditRole).value<QString>());
  }

  {  // Color item
    const core::property property{QColor{Qt::cyan}};

    vm::color_item item;
    EXPECT_NO_THROW(vm::update_item_data(&item, property));

    const auto role = vm::property_item_role::color;
    ASSERT_TRUE(item.data(role).canConvert<QColor>());
    EXPECT_EQ(property.as_color(), item.data(role).value<QColor>());
  }

  {  // File item
    const core::property property{QFileInfo{TACTILE_QSTRING("foo/bar")}};

    vm::file_item item;
    EXPECT_NO_THROW(vm::update_item_data(&item, property));

    const auto role = vm::property_item_role::path;
    ASSERT_TRUE(item.data(role).canConvert<QString>());
    EXPECT_EQ(property.as_file().absoluteFilePath(), item.data(role).value<QString>());
  }

  // TODO object item
}

TEST(PropertyItems, ItemToProperty)
{
  {  // Integer item
    vm::int_item item;
    item.setData(123, Qt::EditRole);

    const auto property = vm::item_to_property(&item);
    ASSERT_TRUE(property.is_integer());
    EXPECT_EQ(123, property.as_integer());
  }

  {  // Floating item
    vm::float_item item;
    item.setData(12.3, Qt::EditRole);

    const auto property = vm::item_to_property(&item);
    ASSERT_TRUE(property.is_floating());
    EXPECT_EQ(12.3, property.as_floating());
  }

  {  // Boolean item
    vm::bool_item item;
    item.setData(Qt::Checked, Qt::CheckStateRole);

    const auto property = vm::item_to_property(&item);
    ASSERT_TRUE(property.is_boolean());
    EXPECT_TRUE(property.as_boolean());
  }

  {  // String item
    vm::string_item item;
    item.setData(TACTILE_QSTRING(u"foo"), Qt::EditRole);

    const auto property = vm::item_to_property(&item);
    ASSERT_TRUE(property.is_string());
    EXPECT_EQ(TACTILE_QSTRING(u"foo"), property.as_string());
  }

  {  // Color item
    vm::color_item item;
    item.setData(QColor{Qt::magenta}, vm::property_item_role::color);

    const auto property = vm::item_to_property(&item);
    ASSERT_TRUE(property.is_color());
    EXPECT_EQ(QColor{Qt::magenta}, property.as_color());
  }

  {  // File item
    vm::file_item item;

    const auto file = TACTILE_QSTRING(u"foo/bar");
    item.setData(file, vm::property_item_role::path);

    const auto property = vm::item_to_property(&item);
    ASSERT_TRUE(property.is_file());
    EXPECT_EQ(QFileInfo{file}.path(), property.as_file().path());
  }

  // TODO object item
}

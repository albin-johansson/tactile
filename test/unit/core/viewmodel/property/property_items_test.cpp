#include "property_items.hpp"

#include <gtest/gtest.h>

#include "tactile_error.hpp"

using namespace tactile;

TEST(PropertyItems, UpdateItemData)
{
  {  // Integer item
    const core::Property property{123};

    vm::IntItem item;
    EXPECT_NO_THROW(vm::UpdateItemData(&item, property));

    ASSERT_TRUE(item.data(Qt::EditRole).canConvert<int>());
    EXPECT_EQ(property.AsInteger(), item.data(Qt::EditRole).value<int>());
  }

  {  // Floating item
    const core::Property property{12.3};

    vm::FloatItem item;
    EXPECT_NO_THROW(vm::UpdateItemData(&item, property));

    ASSERT_TRUE(item.data(Qt::EditRole).canConvert<double>());
    EXPECT_EQ(property.AsFloating(), item.data(Qt::EditRole).value<double>());
  }

  {  // Boolean item
    const core::Property property{true};

    vm::BoolItem item;
    EXPECT_NO_THROW(vm::UpdateItemData(&item, property));

    ASSERT_TRUE(item.data(Qt::CheckStateRole).canConvert<bool>());
    EXPECT_EQ(property.AsBoolean(),
              item.data(Qt::CheckStateRole).value<bool>());
  }

  {  // String item
    const core::Property property{QStringLiteral(u"foo")};

    vm::StringItem item;
    EXPECT_NO_THROW(vm::UpdateItemData(&item, property));

    ASSERT_TRUE(item.data(Qt::EditRole).canConvert<QString>());
    EXPECT_EQ(property.AsString(), item.data(Qt::EditRole).value<QString>());
  }

  {  // Color item
    const core::Property property{QColor{Qt::cyan}};

    vm::ColorItem item;
    EXPECT_NO_THROW(vm::UpdateItemData(&item, property));

    const auto role = vm::PropertyItemRole::Color;
    ASSERT_TRUE(item.data(role).canConvert<QColor>());
    EXPECT_EQ(property.AsColor(), item.data(role).value<QColor>());
  }

  {  // File item
    const core::Property property{QFileInfo{QStringLiteral("foo/bar")}};

    vm::FileItem item;
    EXPECT_NO_THROW(vm::UpdateItemData(&item, property));

    const auto role = vm::PropertyItemRole::Path;
    ASSERT_TRUE(item.data(role).canConvert<QString>());
    EXPECT_EQ(property.AsFile().absoluteFilePath(),
              item.data(role).value<QString>());
  }

  // TODO object item
}

TEST(PropertyItems, ItemToProperty)
{
  {  // Integer item
    vm::IntItem item;
    item.setData(123, Qt::EditRole);

    const auto property = vm::ItemToProperty(&item);
    ASSERT_TRUE(property.IsInteger());
    EXPECT_EQ(123, property.AsInteger());
  }

  {  // Floating item
    vm::FloatItem item;
    item.setData(12.3, Qt::EditRole);

    const auto property = vm::ItemToProperty(&item);
    ASSERT_TRUE(property.IsFloating());
    EXPECT_EQ(12.3, property.AsFloating());
  }

  {  // Boolean item
    vm::BoolItem item;
    item.setData(Qt::Checked, Qt::CheckStateRole);

    const auto property = vm::ItemToProperty(&item);
    ASSERT_TRUE(property.IsBoolean());
    EXPECT_TRUE(property.AsBoolean());
  }

  {  // String item
    vm::StringItem item;
    item.setData(QStringLiteral(u"foo"), Qt::EditRole);

    const auto property = vm::ItemToProperty(&item);
    ASSERT_TRUE(property.IsString());
    EXPECT_EQ(QStringLiteral(u"foo"), property.AsString());
  }

  {  // Color item
    vm::ColorItem item;
    item.setData(QColor{Qt::magenta}, vm::PropertyItemRole::Color);

    const auto property = vm::ItemToProperty(&item);
    ASSERT_TRUE(property.IsColor());
    EXPECT_EQ(QColor{Qt::magenta}, property.AsColor());
  }

  {  // File item
    vm::FileItem item;

    const auto file = QStringLiteral(u"foo/bar");
    item.setData(file, vm::PropertyItemRole::Path);

    const auto property = vm::ItemToProperty(&item);
    ASSERT_TRUE(property.IsFile());
    EXPECT_EQ(QFileInfo{file}.path(), property.AsFile().path());
  }

  // TODO object item
}

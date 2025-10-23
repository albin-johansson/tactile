// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/core/meta/attribute_set.hpp"

#include <utility>

#include <gtest/gtest.h>

#include "tactile/core/container/string.hpp"
#include "tactile/core/container/vector.hpp"

namespace tactile {
namespace {

class AttributeSetTest : public testing::Test
{
 protected:
  AttributeSet m_set {};
};

TEST_F(AttributeSetTest, CreateOrReplace)
{
  EXPECT_EQ(m_set.size(), 0uz);
  EXPECT_EQ(m_set.try_get("A"), nullptr);
  EXPECT_EQ(m_set.try_get("B"), nullptr);

  // Create
  {
    auto& attr = m_set.create_or_replace("A");
    EXPECT_EQ(m_set.size(), 1uz);
    EXPECT_EQ(attr.get_kind(), AttributeKind::kInt);

    auto* const attr_int_ptr = attr.as_int();
    ASSERT_NE(attr_int_ptr, nullptr);
    EXPECT_EQ(*attr_int_ptr, 0);

    *attr_int_ptr = 42;

    EXPECT_NE(m_set.try_get("A"), nullptr);
    EXPECT_EQ(m_set.try_get("B"), nullptr);
  }

  // Replace
  {
    auto& attr = m_set.create_or_replace("A");
    EXPECT_EQ(m_set.size(), 1uz);
    EXPECT_EQ(attr.get_kind(), AttributeKind::kInt);

    auto* const attr_int_ptr = attr.as_int();
    ASSERT_NE(attr_int_ptr, nullptr);
    EXPECT_EQ(*attr_int_ptr, 0);

    EXPECT_NE(m_set.try_get("A"), nullptr);
    EXPECT_EQ(m_set.try_get("B"), nullptr);
  }

  // Create another
  {
    auto& attr = m_set.create_or_replace("B");
    EXPECT_EQ(m_set.size(), 2uz);
    EXPECT_EQ(attr.get_kind(), AttributeKind::kInt);

    auto* const attr_int_ptr = attr.as_int();
    ASSERT_NE(attr_int_ptr, nullptr);
    EXPECT_EQ(*attr_int_ptr, 0);

    EXPECT_NE(m_set.try_get("A"), nullptr);
    EXPECT_NE(m_set.try_get("B"), nullptr);
  }
}

TEST_F(AttributeSetTest, Remove)
{
  EXPECT_EQ(m_set.size(), 0uz);
  EXPECT_EQ(m_set.remove("A"), kNone);

  (void) m_set.create_or_replace("A");
  (void) m_set.create_or_replace("B");
  (void) m_set.create_or_replace("C");
  EXPECT_EQ(m_set.size(), 3uz);

  EXPECT_TRUE(m_set.remove("B").has_value());
  EXPECT_EQ(m_set.size(), 2uz);
  EXPECT_NE(m_set.try_get("A"), nullptr);
  EXPECT_EQ(m_set.try_get("B"), nullptr);
  EXPECT_NE(m_set.try_get("C"), nullptr);

  EXPECT_TRUE(m_set.remove("C").has_value());
  EXPECT_EQ(m_set.size(), 1uz);
  EXPECT_NE(m_set.try_get("A"), nullptr);
  EXPECT_EQ(m_set.try_get("B"), nullptr);
  EXPECT_EQ(m_set.try_get("C"), nullptr);

  EXPECT_TRUE(m_set.remove("A").has_value());
  EXPECT_EQ(m_set.size(), 0uz);
  EXPECT_EQ(m_set.try_get("A"), nullptr);
  EXPECT_EQ(m_set.try_get("B"), nullptr);
  EXPECT_EQ(m_set.try_get("C"), nullptr);
}

TEST_F(AttributeSetTest, TryGet)
{
  EXPECT_EQ(m_set.try_get("A"), nullptr);
  EXPECT_EQ(m_set.try_get("B"), nullptr);
  EXPECT_EQ(std::as_const(m_set).try_get("A"), nullptr);
  EXPECT_EQ(std::as_const(m_set).try_get("B"), nullptr);

  (void) m_set.create_or_replace("A");

  EXPECT_NE(m_set.try_get("A"), nullptr);
  EXPECT_EQ(m_set.try_get("B"), nullptr);
  EXPECT_NE(std::as_const(m_set).try_get("A"), nullptr);
  EXPECT_EQ(std::as_const(m_set).try_get("B"), nullptr);

  (void) m_set.create_or_replace("B");

  EXPECT_NE(m_set.try_get("A"), nullptr);
  EXPECT_NE(m_set.try_get("B"), nullptr);
  EXPECT_NE(std::as_const(m_set).try_get("A"), nullptr);
  EXPECT_NE(std::as_const(m_set).try_get("B"), nullptr);
}

TEST_F(AttributeSetTest, Get)
{
  EXPECT_ANY_THROW((void) m_set.get("A"));
  EXPECT_ANY_THROW((void) std::as_const(m_set).get("A"));

  (void) m_set.create_or_replace("A");

  EXPECT_NO_THROW((void) m_set.get("A"));
  EXPECT_NO_THROW((void) std::as_const(m_set).get("A"));
}

TEST_F(AttributeSetTest, Each)
{
  (void) m_set.create_or_replace("A").emplace<Attribute::int_type>(1);
  (void) m_set.create_or_replace("B").emplace<Attribute::int_type>(2);
  (void) m_set.create_or_replace("C").emplace<Attribute::int_type>(3);

  Vector<String> attr_name_history {};
  Vector<Attribute> attr_value_history {};

  const auto visitor = [&](const String& key, const Attribute& value) {
    attr_name_history.push_back(key);
    attr_value_history.push_back(value);
  };

  m_set.each(visitor);

  ASSERT_EQ(attr_name_history.size(), 3uz);
  ASSERT_EQ(attr_value_history.size(), 3uz);

  EXPECT_EQ(attr_name_history[0uz], "A");
  EXPECT_EQ(attr_name_history[1uz], "B");
  EXPECT_EQ(attr_name_history[2uz], "C");

  ASSERT_EQ(attr_value_history[0uz].get_kind(), AttributeKind::kInt);
  ASSERT_EQ(attr_value_history[1uz].get_kind(), AttributeKind::kInt);
  ASSERT_EQ(attr_value_history[2uz].get_kind(), AttributeKind::kInt);

  EXPECT_EQ(*attr_value_history[0uz].as_int(), 1);
  EXPECT_EQ(*attr_value_history[1uz].as_int(), 2);
  EXPECT_EQ(*attr_value_history[2uz].as_int(), 3);
}

}  // namespace
}  // namespace tactile

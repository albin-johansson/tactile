#include "xml_utils.hpp"

#include <gtest/gtest.h>

#include "tile_id.hpp"

using namespace tactile;

TEST(XmlUtils, GetIntAttr)
{
  QDomDocument document;
  auto elem = document.createElement(QStringLiteral(u"foo"));

  EXPECT_FALSE(GetIntAttr(elem, QStringLiteral(u"bar")));

  const auto name = QStringLiteral(u"Gandalf");
  const auto value = 1'337;

  elem.setAttribute(name, value);
  EXPECT_EQ(value, GetIntAttr(elem, name));
  EXPECT_EQ(tile_id{value}, GetIntAttr<tile_id>(elem, name));
}

TEST(XmlUtils, GetIntAttrDefaultValue)
{
  QDomDocument document;
  auto elem = document.createElement(QStringLiteral(u"foo"));

  EXPECT_NO_THROW(GetIntAttr(elem, QStringLiteral(u"random"), 0));

  const auto value = 8'912;
  EXPECT_EQ(value, GetIntAttr(elem, QStringLiteral(u"xyz"), value));
  EXPECT_EQ(10_t, GetIntAttr<tile_id>(elem, QStringLiteral(u"xyz"), 10_t));
}

TEST(XmlUtils, GetDoubleAttr)
{
  QDomDocument document;
  auto elem = document.createElement(QStringLiteral(u"foo"));

  EXPECT_NO_THROW(GetDoubleAttr(elem, QStringLiteral(u"random"), 0.0));

  const auto value = 27.6;
  EXPECT_EQ(value, GetDoubleAttr(elem, QStringLiteral(u"xyz"), value));

  using mana_type = nenya::strong_type<double, struct mana_t>;
  const mana_type mana{9.2};
  EXPECT_EQ(mana, GetDoubleAttr<mana_type>(elem, QStringLiteral(u"xyz"), mana));
}

TEST(XmlUtils, VisitElements)
{
  QDomDocument document{};

  auto root = document.createElement(QStringLiteral(u"root"));

  const auto tag = QStringLiteral(u"foo");
  root.appendChild(document.createElement(tag));
  root.appendChild(document.createElement(tag));
  root.appendChild(document.createElement(tag));

  document.appendChild(root);

  auto count = 0;
  VisitElements(root, tag, [&](const QDomNode&) { ++count; });

  EXPECT_EQ(3, count);
}

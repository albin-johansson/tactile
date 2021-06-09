#include "xml_utils.hpp"

#include <gtest/gtest.h>

#include "tactile_qstring.hpp"
#include "tile_id.hpp"

using namespace tactile;

TEST(XmlUtils, IntAttr)
{
  QDomDocument document;
  auto elem = document.createElement(TACTILE_QSTRING(u"foo"));

  EXPECT_FALSE(xml::int_attr(elem, TACTILE_QSTRING(u"bar")));

  const auto name = TACTILE_QSTRING(u"Gandalf");
  const auto value = 1'337;

  elem.setAttribute(name, value);
  EXPECT_EQ(value, xml::int_attr(elem, name));
  EXPECT_EQ(tile_id{value}, xml::int_attr<tile_id>(elem, name));
}

TEST(XmlUtils, IntAttrDefaultValue)
{
  QDomDocument document;
  auto elem = document.createElement(TACTILE_QSTRING(u"foo"));

  EXPECT_NO_THROW(xml::int_attr(elem, TACTILE_QSTRING(u"random"), 0));

  const auto value = 8'912;
  EXPECT_EQ(value, xml::int_attr(elem, TACTILE_QSTRING(u"xyz"), value));
  EXPECT_EQ(10_t, xml::int_attr<tile_id>(elem, TACTILE_QSTRING(u"xyz"), 10_t));
}

TEST(XmlUtils, DoubleAttr)
{
  QDomDocument document;
  auto elem = document.createElement(TACTILE_QSTRING(u"foo"));

  EXPECT_NO_THROW(xml::double_attr(elem, TACTILE_QSTRING(u"random"), 0.0));

  const auto value = 27.6;
  EXPECT_EQ(value, xml::double_attr(elem, TACTILE_QSTRING(u"xyz"), value));

  using mana_type = nenya::strong_type<double, struct mana_t>;
  const mana_type mana{9.2};
  EXPECT_EQ(mana,
            xml::double_attr<mana_type>(elem, TACTILE_QSTRING(u"xyz"), mana));
}

TEST(XmlUtils, EachElem)
{
  QDomDocument document{};

  auto root = document.createElement(TACTILE_QSTRING(u"root"));

  const auto tag = TACTILE_QSTRING(u"foo");
  root.appendChild(document.createElement(tag));
  root.appendChild(document.createElement(tag));
  root.appendChild(document.createElement(tag));

  document.appendChild(root);

  auto count = 0;
  xml::each_elem(root, tag, [&](const QDomNode&) {
    ++count;
  });

  EXPECT_EQ(3, count);
}

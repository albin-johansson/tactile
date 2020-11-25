#include "xml_utils.hpp"

#include <catch.hpp>

#include "tile_id.hpp"

using namespace tactile;

TEST_CASE("xml::int_attr", "[xml_utils]")
{
  SECTION("Without default value")
  {
    QDomDocument document;
    auto elem = document.createElement(QStringLiteral(u"foo"));

    CHECK(xml::int_attr(elem, QStringLiteral(u"random")) == std::nullopt);

    const auto name = QStringLiteral(u"kenobi");
    const auto value = 1'337;

    elem.setAttribute(name, value);
    CHECK(xml::int_attr(elem, name) == value);

    SECTION("Convert to strong integer type")
    {
      CHECK(xml::int_attr<tile_id>(elem, name) == tile_id{value});
    }
  }

  SECTION("With default value")
  {
    QDomDocument document;
    auto elem = document.createElement(QStringLiteral(u"foo"));

    CHECK_NOTHROW(xml::int_attr(elem, QStringLiteral(u"random"), 0));

    const auto def = 8912;
    CHECK(xml::int_attr(elem, QStringLiteral(u"xyz"), def) == def);

    SECTION("Using strong integer type")
    {
      CHECK(xml::int_attr<tile_id>(elem, QStringLiteral(u"xyz"), 10_t) == 10_t);
    }
  }
}

TEST_CASE("xml::double_attr", "[xml_utils]")
{
  QDomDocument document;
  auto elem = document.createElement(QStringLiteral(u"foo"));

  CHECK_NOTHROW(xml::double_attr(elem, QStringLiteral(u"random"), 0.0));

  const auto def = 27.6;
  CHECK(xml::double_attr(elem, QStringLiteral(u"xyz"), def) == def);

  SECTION("Using strong double type")
  {
    using mana = nenya::mirror_type<double, struct mana_t>;
    const mana value{9.2};
    CHECK(xml::double_attr<mana>(elem, QStringLiteral(u"xyz"), value) == value);
  }
}

TEST_CASE("xml::each_elem", "[xml_utils]")
{
  QDomDocument document{};
  auto root = document.createElement(QStringLiteral(u"root"));

  root.appendChild(document.createElement(QStringLiteral(u"foo")));
  root.appendChild(document.createElement(QStringLiteral(u"foo")));
  root.appendChild(document.createElement(QStringLiteral(u"foo")));

  document.appendChild(root);

  int count{0};
  xml::each_elem(root, QStringLiteral(u"foo"), [&](const QDomNode& node) {
    ++count;
  });

  CHECK(count == 3);
}

TEST_CASE("xml::from_file", "[xml_utils]")
{
  CHECK_NOTHROW(xml::from_file(QStringLiteral(u"sample.xml")));
  CHECK(xml::from_file(QStringLiteral(u"not_a_file.xml")) == std::nullopt);
}

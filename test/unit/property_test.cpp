#include "property.hpp"

#include <catch.hpp>

using namespace tactile;

TEST_CASE("property::default ctor", "[property]")
{
  const core::property property;
  CHECK(!property.has_value());
  CHECK(!property.get_type());

  CHECK(!property.is_string());
  CHECK(!property.is_integer());
  CHECK(!property.is_floating());
  CHECK(!property.is_boolean());
}

TEST_CASE("property::value ctor", "[property]")
{
  SECTION("Integer property")
  {
    const core::property property{123};
    CHECK(property.is<int>());
    CHECK(property.is_integer());
    CHECK(property.as<int>() == 123);
    CHECK(property.try_as<int>());
    CHECK(property.has_value());

    CHECK(!property.is_floating());
    CHECK(!property.is_string());
    CHECK(!property.is_boolean());
  }

  SECTION("Float property")
  {
    const core::property property{12.3};
    CHECK(property.is<double>());
    CHECK(property.is_floating());
    CHECK(property.as<double>() == 12.3);
    CHECK(property.try_as<double>());
    CHECK(property.has_value());

    CHECK(!property.is_integer());
    CHECK(!property.is_string());
    CHECK(!property.is_boolean());
  }

  SECTION("String property")
  {
    const core::property property{QStringLiteral(u"foo")};
    CHECK(property.is<QString>());
    CHECK(property.is_string());
    CHECK(property.as<QString>() == QStringLiteral(u"foo"));
    CHECK(property.try_as<QString>());
    CHECK(property.has_value());

    CHECK(!property.is_integer());
    CHECK(!property.is_floating());
    CHECK(!property.is_boolean());
  }

  SECTION("Boolean property")
  {
    const core::property property{true};
    CHECK(property.is<bool>());
    CHECK(property.is_boolean());
    CHECK(property.as<bool>());
    CHECK(property.try_as<bool>());
    CHECK(property.has_value());

    CHECK(!property.is_floating());
    CHECK(!property.is_string());
    CHECK(!property.is_string());
  }
}

TEST_CASE("property::reset", "[property]")
{
  core::property property;
  CHECK(!property.has_value());

  property.set_value(123);
  CHECK(property.has_value());

  property.reset();
  CHECK(!property.has_value());

  CHECK_NOTHROW(property.reset());
  CHECK_NOTHROW(property.reset());
}

TEST_CASE("property::set_value", "[property]")
{
  core::property property;
  CHECK(!property.has_value());

  property.set_value(10);
  CHECK(property.has_value());
  CHECK(property.as<int>() == 10);

  property.set_value(true);
  CHECK(property.has_value());
  CHECK(property.as<bool>());

  property.set_value(QStringLiteral(u"foo"));
  CHECK(property.is<QString>());
  CHECK(property.as<QString>() == QStringLiteral(u"foo"));

  property.set_value(93.2);
  CHECK(property.is<double>());
  CHECK(property.as<double>() == 93.2);
}

TEST_CASE("property::is", "[property]")
{
  core::property property;

  CHECK(!property.is<int>());
  CHECK(!property.is<double>());
  CHECK(!property.is<QString>());
  CHECK(!property.is<bool>());

  property.set_value(34);
  CHECK(property.is<int>());
  CHECK(!property.is<double>());
  CHECK(!property.is<QString>());
  CHECK(!property.is<bool>());

  property.set_value(83.2);
  CHECK(!property.is<int>());
  CHECK(property.is<double>());
  CHECK(!property.is<QString>());
  CHECK(!property.is<bool>());

  property.set_value(QStringLiteral(u"foobar"));
  CHECK(!property.is<int>());
  CHECK(!property.is<double>());
  CHECK(property.is<QString>());
  CHECK(!property.is<bool>());

  property.set_value(false);
  CHECK(!property.is<int>());
  CHECK(!property.is<double>());
  CHECK(!property.is<QString>());
  CHECK(property.is<bool>());
}

TEST_CASE("property::has_value", "[property]")
{
  core::property property;
  CHECK(!property.has_value());

  property.set_value(924);
  CHECK(property.has_value());
}

TEST_CASE("property::as", "[property]")
{
  core::property property;

  CHECK_THROWS(property.as<int>());
  CHECK_THROWS(property.as<double>());
  CHECK_THROWS(property.as<QString>());
  CHECK_THROWS(property.as<bool>());

  property.set_value(123);
  CHECK_NOTHROW(property.as<int>());
  CHECK_THROWS(property.as<double>());
  CHECK_THROWS(property.as<QString>());
  CHECK_THROWS(property.as<bool>());
}

TEST_CASE("property::try_as", "[property]")
{
  core::property property;

  CHECK(!property.try_as<int>());
  CHECK(!property.try_as<double>());
  CHECK(!property.try_as<QString>());
  CHECK(!property.try_as<bool>());

  property.set_value(123);
  CHECK(property.as<int>());
  CHECK(!property.try_as<double>());
  CHECK(!property.try_as<QString>());
  CHECK(!property.try_as<bool>());
}

TEST_CASE("property::get_type", "[property]")
{
  core::property property;
  CHECK(!property.get_type());

  property.set_value(123);
  CHECK(property.get_type() == core::property::integer);

  property.set_value(12.3);
  CHECK(property.get_type() == core::property::floating);

  property.set_value(QStringLiteral(u"foo"));
  CHECK(property.get_type() == core::property::string);

  property.set_value(true);
  CHECK(property.get_type() == core::property::boolean);

  property.reset();
  CHECK(!property.get_type());
}

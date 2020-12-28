#include "property_delegate.hpp"

#include <catch.hpp>

#include "tactile_qstring.hpp"

using namespace tactile;

TEST_CASE("property_delegate::add_property", "[property_delegate]")
{
  core::property_delegate delegate;
  CHECK(delegate.property_count() == 0);

  const auto name = TACTILE_QSTRING(u"foo");
  delegate.add_property(name, core::property::integer);
  CHECK(delegate.property_count() == 1);

  const auto& property = delegate.get_property(name);
  CHECK(property.is_integer());
  CHECK(property.as_integer() == 0);
}

TEST_CASE("property_delegate::remove_property", "[property_delegate]")
{
  core::property_delegate delegate;

  const auto name = TACTILE_QSTRING(u"foo");

  delegate.add_property(name, core::property::integer);
  CHECK(delegate.property_count() == 1);
  CHECK_NOTHROW(delegate.get_property(name));

  delegate.remove_property(name);
  CHECK(delegate.property_count() == 0);
  CHECK_THROWS(delegate.get_property(name));
}

TEST_CASE("property_delegate::rename_property", "[property_delegate]")
{
  core::property_delegate delegate;

  const auto oldName = TACTILE_QSTRING(u"foo");
  delegate.add_property(oldName, core::property::boolean);
  CHECK_NOTHROW(delegate.get_property(oldName));

  const auto newName = TACTILE_QSTRING(u"bar");
  CHECK_NOTHROW(delegate.rename_property(oldName, newName));
  CHECK_THROWS(delegate.get_property(oldName));

  const auto& property = delegate.get_property(newName);
  CHECK(property.is_boolean());
}

TEST_CASE("property_delegate::set_property", "[property_delegate]")
{
  core::property_delegate delegate;

  const auto name = TACTILE_QSTRING(u"foo");

  delegate.add_property(name, core::property::integer);
  CHECK(delegate.get_property(name).as_integer() == 0);

  delegate.set_property(name, 123);
  CHECK(delegate.get_property(name).as_integer() == 123);
}

TEST_CASE("property_delegate::get_property", "[property_delegate]")
{
  core::property_delegate delegate;

  const auto name = TACTILE_QSTRING(u"foo");
  CHECK_THROWS(delegate.get_property(name));

  delegate.add_property(name, core::property::string);
  CHECK_NOTHROW(delegate.get_property(name));

  const auto& cDelegate = delegate;
  CHECK_NOTHROW(cDelegate.get_property(name));
}

TEST_CASE("property_delegate::property_count", "[property_delegate]")
{
  core::property_delegate delegate;
  CHECK(delegate.property_count() == 0);

  delegate.add_property(TACTILE_QSTRING(u"foo"), core::property::string);
  CHECK(delegate.property_count() == 1);

  delegate.add_property(TACTILE_QSTRING(u"bar"), core::property::integer);
  CHECK(delegate.property_count() == 2);

  delegate.remove_property(TACTILE_QSTRING(u"bar"));
  CHECK(delegate.property_count() == 1);

  delegate.remove_property(TACTILE_QSTRING(u"foo"));
  CHECK(delegate.property_count() == 0);
}
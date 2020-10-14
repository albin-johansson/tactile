#include "string_utils.hpp"

#include <catch.hpp>

using namespace tactile;

TEST_CASE("to_int", "[string_utils]")
{
  CHECK(to_int(QStringLiteral(u"1")) == 1);
  CHECK(to_int(QStringLiteral(u"-632")) == -632);
  CHECK(to_int(QStringLiteral(u"1337")) == 1'337);
  CHECK_THROWS(to_int(QStringLiteral(u"A")));
  CHECK_THROWS(to_int(QStringLiteral(u"abc")));
}

#include "json_utils.hpp"

#include <catch.hpp>

using namespace tactile;

TEST_CASE("json::from_file", "[json_utils]")
{
  CHECK_NOTHROW(json::from_file(QStringLiteral(u"json/terrain.json")));
}

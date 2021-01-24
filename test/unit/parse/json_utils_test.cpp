#include "json_utils.hpp"

#include <gtest/gtest.h>

#include "tactile_qstring.hpp"

using namespace tactile;

TEST(JSONUtils, FromFile)
{
  EXPECT_NO_THROW(json::from_file(TACTILE_QSTRING(u"json/terrain.json")));
}

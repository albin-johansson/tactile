#include "json_utils.hpp"

#include <gtest/gtest.h>

#include "tactile_qstring.hpp"

using namespace tactile;

TEST(JSONUtils, ReadJson)
{
  EXPECT_NO_THROW(ReadJson(QFileInfo{TACTILE_QSTRING(u"json/terrain.json")}));
}

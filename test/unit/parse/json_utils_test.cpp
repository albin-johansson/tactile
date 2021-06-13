#include "json_utils.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(JSONUtils, ReadJson)
{
  EXPECT_NO_THROW(ReadJson(QFileInfo{QStringLiteral(u"json/terrain.json")}));
}

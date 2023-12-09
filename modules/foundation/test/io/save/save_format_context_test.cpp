// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tactile/foundation/io/save/save_format_context.hpp"

using namespace tactile;
using testing::Return;

namespace {

// NOLINTBEGIN(modernize-use-trailing-return-type)

class MockSaveFormat final : public ISaveFormat {
 public:
  MOCK_METHOD(Result<ir::Map>,
              load_map,
              (const FilePath&, const SaveFormatReadOptions&),
              (const));

  MOCK_METHOD(Result<ir::Tileset>,
              load_tileset,
              (const FilePath&, const SaveFormatReadOptions&),
              (const));

  MOCK_METHOD(Result<void>,
              save_map,
              (const FilePath&, const ir::Map&, const SaveFormatWriteOptions&),
              (const));

  MOCK_METHOD(Result<void>,
              save_tileset,
              (const FilePath&, const ir::Tileset&, const SaveFormatWriteOptions&),
              (const));

  MOCK_METHOD(bool, is_valid_extension, (NativeStringView), (const));
};

// NOLINTEND(modernize-use-trailing-return-type)

}  // namespace

TEST(SaveFormatContext, LoadMap)
{
  MockSaveFormat save_format_x {};
  MockSaveFormat save_format_y {};

  ON_CALL(save_format_x, is_valid_extension)
      .WillByDefault([](NativeStringView extension) {
        return extension == TACTILE_NATIVE_STR(".x");
      });

  ON_CALL(save_format_y, is_valid_extension)
      .WillByDefault([](NativeStringView extension) {
        return extension == TACTILE_NATIVE_STR(".y");
      });

  SaveFormatContext save_format_context {};
  save_format_context.add_format(&save_format_x);
  save_format_context.add_format(&save_format_y);

  EXPECT_CALL(save_format_x, is_valid_extension).WillRepeatedly(Return(false));
  EXPECT_CALL(save_format_y, is_valid_extension)
      .WillOnce(Return(false))
      .WillOnce(Return(true));
  EXPECT_CALL(save_format_x, load_map).Times(0);
  EXPECT_CALL(save_format_y, load_map).Times(1);

  (void) save_format_context.load_map("foo.w", SaveFormatReadOptions {});
  (void) save_format_context.load_map("foo.y", SaveFormatReadOptions {});
}

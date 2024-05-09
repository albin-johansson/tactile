// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/imgui_compat.hpp"

#include <gtest/gtest.h>

namespace tactile {

/// \trace tactile::to_float2
TEST(ImGuiCompat, ToFloat2)
{
  const ImVec2 input {-10.0f, 20.0f};
  const Float2 output = to_float2(input);

  EXPECT_EQ(output.x(), input.x);
  EXPECT_EQ(output.y(), input.y);
}

/// \trace tactile::to_float4
TEST(ImGuiCompat, ToFloat4)
{
  const ImVec4 input {1.0f, 2.0f, 3.0f, 4.0f};
  const Float4 output = to_float4(input);

  EXPECT_EQ(output.x(), input.x);
  EXPECT_EQ(output.y(), input.y);
  EXPECT_EQ(output.z(), input.z);
  EXPECT_EQ(output.w(), input.w);
}

/// \trace tactile::to_int2
TEST(ImGuiCompat, ToInt2)
{
  const ImVec2 input {-3.4f, 9.7f};
  const Int2 output = to_int2(input);

  EXPECT_EQ(output.x(), static_cast<Int4::value_type>(input.x));
  EXPECT_EQ(output.y(), static_cast<Int4::value_type>(input.y));
}

/// \trace tactile::to_int4
TEST(ImGuiCompat, ToInt4)
{
  const ImVec4 input {1.5f, 2.1f, 3.6f, 4.9f};
  const Int4 output = to_int4(input);

  EXPECT_EQ(output.x(), static_cast<Int4::value_type>(input.x));
  EXPECT_EQ(output.y(), static_cast<Int4::value_type>(input.y));
  EXPECT_EQ(output.z(), static_cast<Int4::value_type>(input.z));
  EXPECT_EQ(output.w(), static_cast<Int4::value_type>(input.w));
}

/// \trace tactile::to_imgui_data_type
TEST(ImGuiCompat, ToImGuiDataType)
{
  EXPECT_EQ(to_imgui_data_type<float>(), ImGuiDataType_Float);
  EXPECT_EQ(to_imgui_data_type<double>(), ImGuiDataType_Double);

  EXPECT_EQ(to_imgui_data_type<uint8>(), ImGuiDataType_U8);
  EXPECT_EQ(to_imgui_data_type<uint16>(), ImGuiDataType_U16);
  EXPECT_EQ(to_imgui_data_type<uint32>(), ImGuiDataType_U32);
  EXPECT_EQ(to_imgui_data_type<uint64>(), ImGuiDataType_U64);

  EXPECT_EQ(to_imgui_data_type<int8>(), ImGuiDataType_S8);
  EXPECT_EQ(to_imgui_data_type<int16>(), ImGuiDataType_S16);
  EXPECT_EQ(to_imgui_data_type<int32>(), ImGuiDataType_S32);
  EXPECT_EQ(to_imgui_data_type<int64>(), ImGuiDataType_S64);
}

}  // namespace tactile

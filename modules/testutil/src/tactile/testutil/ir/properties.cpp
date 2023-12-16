// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/testutil/ir/properties.hpp"

namespace tactile::testutil {

auto make_basic_properties() -> Vector<ir::NamedAttribute>
{
  Vector<ir::NamedAttribute> properties {};
  properties.reserve(7);

  properties.push_back({"test-string", "foobar"});
  properties.push_back({"test-int", 123});
  properties.push_back({"test-float", 1.2f});
  properties.push_back({"test-bool", true});
  properties.push_back({"test-path", FilePath {"a/b/c"}});
  properties.push_back({"test-color", UColor {0x1A, 0x2B, 0x3C, 0x4D}});
  properties.push_back({"test-object", ObjectRef {99}});

  return properties;
}

auto make_all_properties() -> Vector<ir::NamedAttribute>
{
  auto properties = make_basic_properties();
  properties.reserve(13);

  properties.push_back({"test-int2", Int2 {12, 34}});
  properties.push_back({"test-int3", Int3 {12, 34, 56}});
  properties.push_back({"test-int4", Int4 {12, 34, 56, 78}});
  properties.push_back({"test-float2", Float2 {1.2f, 3.4f}});
  properties.push_back({"test-float3", Float3 {1.2f, 3.4f, 5.6f}});
  properties.push_back({"test-float4", Float4 {1.2f, 3.4f, 5.6f, 7.8f}});

  return properties;
}

}  // namespace tactile::testutil

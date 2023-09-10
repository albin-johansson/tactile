// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>  // same_as, convertible_to
#include <ostream>   // ostream
#include <utility>   // move

#include "tactile/core/api.hpp"
#include "tactile/core/context/attribute_type.hpp"
#include "tactile/core/context/color.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/debug/error.hpp"
#include "tactile/core/io/filesystem.hpp"
#include "tactile/core/math/vector.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/type/maybe.hpp"
#include "tactile/core/type/string.hpp"
#include "tactile/core/type/variant.hpp"

namespace tactile {

template <typename T>
concept AttributeValueType = std::same_as<T, bool> ||            //
                             std::convertible_to<T, String> ||   //
                             std::convertible_to<T, int32> ||    //
                             std::convertible_to<T, Int2> ||     //
                             std::convertible_to<T, Int3> ||     //
                             std::convertible_to<T, Int4> ||     //
                             std::convertible_to<T, float32> ||  //
                             std::convertible_to<T, Float2> ||   //
                             std::convertible_to<T, Float3> ||   //
                             std::convertible_to<T, Float4> ||   //
                             std::convertible_to<T, UColor> ||   //
                             std::convertible_to<T, Path> ||     //
                             std::convertible_to<T, ObjectRef>;

/** Represents a generic value of one of several possible types. */
class TACTILE_CORE_API Attribute final {
  // These are indices into the value type variant
  inline static constexpr usize kStringTypeIndex = 0;
  inline static constexpr usize kIntTypeIndex = 1;
  inline static constexpr usize kInt2TypeIndex = 2;
  inline static constexpr usize kInt3TypeIndex = 3;
  inline static constexpr usize kInt4TypeIndex = 4;
  inline static constexpr usize kFloatTypeIndex = 5;
  inline static constexpr usize kFloat2TypeIndex = 6;
  inline static constexpr usize kFloat3TypeIndex = 7;
  inline static constexpr usize kFloat4TypeIndex = 8;
  inline static constexpr usize kBoolTypeIndex = 9;
  inline static constexpr usize kColorTypeIndex = 10;
  inline static constexpr usize kPathTypeIndex = 11;
  inline static constexpr usize kObjRefTypeIndex = 12;

 public:
  using string_type = String;
  using int_type = int32;
  using int2_type = Int2;
  using int3_type = Int3;
  using int4_type = Int4;
  using float_type = float32;
  using float2_type = Float2;
  using float3_type = Float3;
  using float4_type = Float4;
  using color_type = UColor;
  using path_type = Path;
  using objref_type = ObjectRef;

  // Remember to update the type indices if the type order changes.
  using variant_type = Variant<string_type,
                               int_type,
                               int2_type,
                               int3_type,
                               int4_type,
                               float_type,
                               float2_type,
                               float3_type,
                               float4_type,
                               bool,
                               color_type,
                               path_type,
                               objref_type>;

  /** Creates an empty string attribute. */
  Attribute() = default;

  /** Creates an attribute with the default value of the specified type. */
  explicit Attribute(AttributeType type);

  template <AttributeValueType T>
  Attribute(T value)
  {
    mValue.emplace<T>(std::move(value));
  }

  Attribute(const char* str)
  {
    TACTILE_ASSERT(str);
    mValue.emplace<string_type>(str);
  }

  /** Resets the attribute the default value for the specified type. */
  void reset(AttributeType type);

  template <AttributeValueType T>
  void set(T value)
  {
    mValue.emplace<T>(std::move(value));
  }

  void set(const char* str) { mValue.emplace<string_type>(str); }

  [[nodiscard]] auto as_string() const -> const string_type&;

  [[nodiscard]] auto as_int() const -> int_type;

  [[nodiscard]] auto as_int2() const -> const int2_type&;

  [[nodiscard]] auto as_int3() const -> const int3_type&;

  [[nodiscard]] auto as_int4() const -> const int4_type&;

  [[nodiscard]] auto as_float() const -> float_type;

  [[nodiscard]] auto as_float2() const -> const float2_type&;

  [[nodiscard]] auto as_float3() const -> const float3_type&;

  [[nodiscard]] auto as_float4() const -> const float4_type&;

  [[nodiscard]] auto as_bool() const -> bool;

  [[nodiscard]] auto as_color() const -> const color_type&;

  [[nodiscard]] auto as_path() const -> const path_type&;

  [[nodiscard]] auto as_object() const -> objref_type;

  [[nodiscard]] auto is_vector() const -> bool;

  [[nodiscard]] auto has_default_value() const -> bool;

  [[nodiscard]] auto get_type() const -> AttributeType;

  [[nodiscard]] auto operator==(const Attribute&) const -> bool = default;

 private:
  variant_type mValue;

  template <AttributeValueType T>
  [[nodiscard]] auto _get() const -> const T&
  {
    if (const auto* value = get_if<T>(&mValue)) {
      return *value;
    }
    else {
      throw Error {"Attribute type mismatch"};
    }
  }
};

TACTILE_CORE_API
auto operator<<(std::ostream& stream, const Attribute& attribute)
    -> std::ostream&;

}  // namespace tactile

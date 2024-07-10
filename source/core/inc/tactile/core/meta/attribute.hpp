// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>  // same_as, convertible_to
#include <utility>   // move

#include "tactile/base/container/path.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/container/variant.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/meta/color.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/debug/exception.hpp"
#include "tactile/core/meta/attribute_type.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

template <typename T>
concept AttributeValueType = std::same_as<T, bool> ||           //
                             std::convertible_to<T, String> ||  //
                             std::convertible_to<T, int32> ||   //
                             std::convertible_to<T, Int2> ||    //
                             std::convertible_to<T, Int3> ||    //
                             std::convertible_to<T, Int4> ||    //
                             std::convertible_to<T, float> ||   //
                             std::convertible_to<T, Float2> ||  //
                             std::convertible_to<T, Float3> ||  //
                             std::convertible_to<T, Float4> ||  //
                             std::convertible_to<T, UColor> ||  //
                             std::convertible_to<T, Path> ||    //
                             std::convertible_to<T, ObjectRef>;

/**
 * Represents a generic value of one of several possible types.
 */
class Attribute final
{
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
  using float_type = float;
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

  /**
   * Creates an empty string attribute.
   */
  Attribute() = default;

  /**
   * Creates an attribute with the default value of the specified type.
   *
   * \param type The underlying value type.
   */
  explicit Attribute(AttributeType type);

  /**
   * Creates an attribute with a specific value.
   *
   * \param value The initial value.
   */
  template <AttributeValueType T>
  explicit Attribute(T value)
  {
    mValue.emplace<T>(std::move(value));
  }

  /**
   * Creates a string attribute.
   *
   * \pre The provided string cannot be null.
   *
   * \param str A C-style string.
   */
  explicit Attribute(const char* str);

  /**
   * Resets the attribute the default value for the specified type.
   *
   * \param type The underlying value type.
   */
  void reset(AttributeType type);

  /**
   * Updates the value of the attribute.
   *
   * \param value The new value.
   */
  template <AttributeValueType T>
  void set(T value)
  {
    mValue.emplace<T>(std::move(value));
  }

  /**
   * Updates the value of the attribute.
   *
   * \pre The provided string cannot be null.
   *
   * \param str The new string value.
   */
  void set(const char* str);

  /**
   * Returns the underlying string value.
   *
   * \return
   * The underlying value.
   *
   * \throws Exception if the attribute is of a different type.
   */
  [[nodiscard]]
  auto as_string() const -> const string_type&;

  /**
   * Returns the underlying int value.
   *
   * \return
   * The underlying value.
   *
   * \throws Exception if the attribute is of a different type.
   */
  [[nodiscard]]
  auto as_int() const -> int_type;

  /**
   * Returns the underlying 2D int vector value.
   *
   * \return
   * The underlying value.
   *
   * \throws Exception if the attribute is of a different type.
   */
  [[nodiscard]]
  auto as_int2() const -> const int2_type&;

  /**
   * Returns the underlying 3D int vector value.
   *
   * \return
   * The underlying value.
   *
   * \throws Exception if the attribute is of a different type.
   */
  [[nodiscard]]
  auto as_int3() const -> const int3_type&;

  /**
   * Returns the underlying 4D int vector value.
   *
   * \return
   * The underlying value.
   *
   * \throws Exception if the attribute is of a different type.
   */
  [[nodiscard]]
  auto as_int4() const -> const int4_type&;

  /**
   * Returns the underlying float value.
   *
   * \return
   * The underlying value.
   *
   * \throws Exception if the attribute is of a different type.
   */
  [[nodiscard]]
  auto as_float() const -> float_type;

  /**
   * Returns the underlying 2D float vector value.
   *
   * \return
   * The underlying value.
   *
   * \throws Exception if the attribute is of a different type.
   */
  [[nodiscard]]
  auto as_float2() const -> const float2_type&;

  /**
   * Returns the underlying 3D float vector value.
   *
   * \return
   * The underlying value.
   *
   * \throws Exception if the attribute is of a different type.
   */
  [[nodiscard]]
  auto as_float3() const -> const float3_type&;

  /**
   * Returns the underlying 4D float vector value.
   *
   * \return
   * The underlying value.
   *
   * \throws Exception if the attribute is of a different type.
   */
  [[nodiscard]]
  auto as_float4() const -> const float4_type&;

  /**
   * Returns the underlying boolean value.
   *
   * \return
   * The underlying value.
   *
   * \throws Exception if the attribute is of a different type.
   */
  [[nodiscard]]
  auto as_bool() const -> bool;

  /**
   * Returns the underlying color value.
   *
   * \return
   * The underlying value.
   *
   * \throws Exception if the attribute is of a different type.
   */
  [[nodiscard]]
  auto as_color() const -> const color_type&;

  /**
   * Returns the underlying file path value.
   *
   * \return
   * The underlying value.
   *
   * \throws Exception if the attribute is of a different type.
   */
  [[nodiscard]]
  auto as_path() const -> const path_type&;

  /**
   * Returns the underlying object reference value.
   *
   * \return
   * The underlying value.
   *
   * \throws Exception if the attribute is of a different type.
   */
  [[nodiscard]]
  auto as_object() const -> objref_type;

  /**
   * Indicates whether the attribute is a vector attribute.
   *
   * \return
   * True if the attribute has a vector value; false otherwise.
   */
  [[nodiscard]]
  auto is_vector() const -> bool;

  /**
   * Indicates whether the attribute has the default value for its type.
   *
   * \return
   * True if the value is the default one; false otherwise.
   */
  [[nodiscard]]
  auto has_default_value() const -> bool;

  /**
   * Returns the type of the current value.
   *
   * \return
   * The current type.
   */
  [[nodiscard]]
  auto get_type() const -> AttributeType;

  [[nodiscard]] auto operator==(const Attribute&) const -> bool = default;

 private:
  variant_type mValue {};

  template <AttributeValueType T>
  [[nodiscard]] auto _get() const -> const T&
  {
    if (const auto* value = get_if<T>(&mValue)) {
      return *value;
    }

    throw Exception {"bad attribute type"};
  }
};

}  // namespace tactile

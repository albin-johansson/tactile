// Copyright (C) 2025 Albin Johansson

export module tactile.core.meta:attr;

export import std;
export import tactile.core.common;
export import tactile.core.numeric;
export import :color;

export namespace tactile {

/// Represents different kinds of attributes.
enum class AttrKind : u8
{
  kInt,
  kInt2,
  kInt3,
  kInt4,
  kFloat,
  kFloat2,
  kFloat3,
  kFloat4,
  kBool,
  kString,
  kPath,
  kColor,
};

/// Represents an attribute value.
class Attr final
{
 public:
  using int_type = i32;
  using int2_type = Int2;
  using int3_type = Int3;
  using int4_type = Int4;
  using float_type = f32;
  using float2_type = Float2;
  using float3_type = Float3;
  using float4_type = Float4;
  using string_type = String;
  using path_type = Path;
  using color_type = Color;

  /// Creates an attribute of a given kind.
  explicit Attr(AttrKind kind = AttrKind::kInt);

  /// Resets the value of the attribute to default value of a given attribute
  /// kind.
  void reset(AttrKind kind);

  /// Constructs a value in the attribute in-place.
  template <typename T, typename... Args>
  auto emplace(Args&&... args) -> T&
  {
    return m_value.emplace<T>(std::forward<Args>(args)...);
  }

  /// Returns a pointer to the underlying integer value, if there is one.
  [[nodiscard]]
  auto as_int() -> int_type*;

  /// Returns a pointer to the underlying integer value, if there is one.
  [[nodiscard]]
  auto as_int() const -> const int_type*;

  /// Returns a pointer to underlying 2D integer vector value, if there is one.
  [[nodiscard]]
  auto as_int2() -> int2_type*;

  /// Returns a pointer to underlying 2D integer vector value, if there is one.
  [[nodiscard]]
  auto as_int2() const -> const int2_type*;

  /// Returns a pointer to underlying 3D integer vector value, if there is one.
  [[nodiscard]]
  auto as_int3() -> int3_type*;

  /// Returns a pointer to underlying 3D integer vector value, if there is one.
  [[nodiscard]]
  auto as_int3() const -> const int3_type*;

  /// Returns a pointer to underlying 4D integer vector value, if there is one.
  [[nodiscard]]
  auto as_int4() -> int4_type*;

  /// Returns a pointer to underlying 4D integer vector value, if there is one.
  [[nodiscard]]
  auto as_int4() const -> const int4_type*;

  /// Returns a pointer to underlying float value, if there is one.
  [[nodiscard]]
  auto as_float() -> float_type*;

  /// Returns a pointer to underlying float value, if there is one.
  [[nodiscard]]
  auto as_float() const -> const float_type*;

  /// Returns a pointer to underlying 2D float vector value, if there is one.
  [[nodiscard]]
  auto as_float2() -> float2_type*;

  /// Returns a pointer to underlying 2D float vector value, if there is one.
  [[nodiscard]]
  auto as_float2() const -> const float2_type*;

  /// Returns a pointer to underlying 3D float vector value, if there is one.
  [[nodiscard]]
  auto as_float3() -> float3_type*;

  /// Returns a pointer to underlying 3D float vector value, if there is one.
  [[nodiscard]]
  auto as_float3() const -> const float3_type*;

  /// Returns a pointer to underlying 4D float vector value, if there is one.
  [[nodiscard]]
  auto as_float4() -> float4_type*;

  /// Returns a pointer to underlying 4D float vector value, if there is one.
  [[nodiscard]]
  auto as_float4() const -> const float4_type*;

  /// Returns a pointer to underlying boolean value, if there is one.
  [[nodiscard]]
  auto as_bool() -> bool*;

  /// Returns a pointer to underlying boolean value, if there is one.
  [[nodiscard]]
  auto as_bool() const -> const bool*;

  /// Returns a pointer to underlying string value, if there is one.
  [[nodiscard]]
  auto as_string() -> string_type*;

  /// Returns a pointer to underlying string value, if there is one.
  [[nodiscard]]
  auto as_string() const -> const string_type*;

  /// Returns a pointer to underlying path value, if there is one.
  [[nodiscard]]
  auto as_path() -> path_type*;

  /// Returns a pointer to underlying path value, if there is one.
  [[nodiscard]]
  auto as_path() const -> const path_type*;

  /// Returns a pointer to underlying color value, if there is one.
  [[nodiscard]]
  auto as_color() -> color_type*;

  /// Returns a pointer to underlying color value, if there is one.
  [[nodiscard]]
  auto as_color() const -> const color_type*;

  /// Returns the kind of the attribute.
  [[nodiscard]]
  auto kind() const -> AttrKind;

  /// Indicates whether the attribute has the default value for its kind.
  [[nodiscard]]
  auto has_default_value() const -> bool
  {
    const auto visitor = []<typename T>(const T& value) -> bool {
      return value == T {};
    };
    return std::visit(visitor, m_value);
  }

  /// Indicates whether the attribute has a vector value.
  [[nodiscard]]
  auto is_vector() const -> bool;

  [[nodiscard]]
  auto operator==(const Attr&) const -> bool = default;

 private:
  // These are indices into the value_type variant type parameters.
  constexpr static usize kIntTypeIndex {0};
  constexpr static usize kInt2TypeIndex {1};
  constexpr static usize kInt3TypeIndex {2};
  constexpr static usize kInt4TypeIndex {3};
  constexpr static usize kFloatTypeIndex {4};
  constexpr static usize kFloat2TypeIndex {5};
  constexpr static usize kFloat3TypeIndex {6};
  constexpr static usize kFloat4TypeIndex {7};
  constexpr static usize kBoolTypeIndex {8};
  constexpr static usize kStringTypeIndex {9};
  constexpr static usize kPathTypeIndex {10};
  constexpr static usize kColorTypeIndex {11};

  using value_type = std::variant<int_type,
                                  int2_type,
                                  int3_type,
                                  int4_type,
                                  float_type,
                                  float2_type,
                                  float3_type,
                                  float4_type,
                                  bool,
                                  string_type,
                                  path_type,
                                  color_type>;

  value_type m_value {};
};

}  // namespace tactile

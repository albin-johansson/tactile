// Copyright (C) 2025 Albin Johansson

export module tactile.core.util:concepts;

import tactile.core.ext.std;

export namespace tactile {

/// Concept that accepts one of the given types.
template <typename T, typename... Ts>
concept OneOf = (std::same_as<T, Ts> || ...);

/// Concept that accepts types without any CV-qualifiers.
template <typename T>
concept WithoutCV = !std::is_const_v<T> && !std::is_volatile_v<T>;

/// Concept that accepts proper integer types (see P3701R0).
template <typename T>
concept Integer =
    std::integral<T> &&
    !OneOf<T, bool, char, wchar_t, char8_t, char16_t, char32_t> && WithoutCV<T>;

/// Concept that accepts proper signed integer types (see P3701R0).
template <typename T>
concept SignedInteger = Integer<T> && std::is_signed_v<T>;

/// Concept that accepts proper unsigned integer types (see P3701R0).
template <typename T>
concept UnsignedInteger = Integer<T> && std::is_unsigned_v<T>;

template <typename T>
concept Numeric = Integer<T> || std::floating_point<T>;

}  // namespace tactile

#ifndef RUNE_CORE_COMPILER_HPP
#define RUNE_CORE_COMPILER_HPP

namespace rune {

/// \addtogroup core
/// \{

/// \name Compiler checks
/// \{

/// Indicates whether or not the current compiler is MSVC
[[nodiscard]] consteval auto on_msvc() noexcept -> bool
{
#ifdef _MSC_VER
  return true;
#else
  return false;
#endif  // _MSC_VER
}

/// Indicates whether or not the current compiler is GCC
[[nodiscard]] consteval auto on_gcc() noexcept -> bool
{
#ifdef __GNUC__
  return true;
#else
  return false;
#endif  // __GNUC__
}

/// Indicates whether or not the current compiler is Clang
[[nodiscard]] consteval auto on_clang() noexcept -> bool
{
#ifdef __clang__
  return true;
#else
  return false;
#endif  // __clang__
}

/// \} End of compiler checks

/// \} End of group core

}  // namespace rune

#endif  // RUNE_CORE_COMPILER_HPP

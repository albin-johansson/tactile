#ifndef RUNE_MAYBE_HPP_
#define RUNE_MAYBE_HPP_

#include <optional>  // optional

namespace rune {

template <typename T>
using maybe = std::optional<T>;

inline constexpr std::nullopt_t nothing = std::nullopt;

}  // namespace rune

#endif  // RUNE_MAYBE_HPP_

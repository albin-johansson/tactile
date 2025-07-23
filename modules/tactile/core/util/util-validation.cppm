// Copyright (C) 2025 Albin Johansson

export module tactile.core.util:validation;

import tactile.core.ext.std;

export namespace tactile {

/// Returns a given pointer if it's not null, throws an exception otherwise.
template <typename T>
[[nodiscard]] auto require_not_null(T ptr) -> T
{
  if (ptr == nullptr) [[unlikely]] {
    throw std::invalid_argument {"unexpected null pointer"};
  }
  return ptr;
}

}  // namespace tactile

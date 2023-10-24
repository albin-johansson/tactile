// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <memory>  // unique_ptr, shared_ptr, weak_ptr, default_delete, make_unique, make_shared
#include <utility>  // forward, move

namespace tactile {

template <typename T, typename Deleter = std::default_delete<T>>
using Unique = std::unique_ptr<T, Deleter>;

template <typename T>
using Shared = std::shared_ptr<T>;

template <typename T>
using Weak = std::weak_ptr<T>;

template <typename T>
using DeleterFptr = void (*)(T*);

/** \brief Unique pointer with a function pointer deleter, useful for externally managed pointers. */
template <typename T>
using Managed = Unique<T, DeleterFptr<T>>;

using std::make_shared;
using std::make_unique;

/**
 * \brief Creates a managed object.
 *
 * \tparam T    the type of the object to allocate.
 * \tparam Args the types of the forwarded arguments.
 *
 * \param args the arguments that will be forwarded to an appropriate constructor.
 *
 * \return a managed pointer.
 */
template <typename T, typename... Args>
[[nodiscard]] auto make_managed(Args&&... args) -> Managed<T>
{
  auto deleter = [](T* ptr) noexcept { delete ptr; };
  return Managed<T> {new T {std::forward<Args>(args)...}, deleter};
}

/**
 * \brief Converts a managed pointer to a derived type to one to a base type.
 *
 * \pre The deleter used by the provided managed pointer must call `operator delete`.
 *
 * \tparam Base    the base type.
 * \tparam Derived the derived type.
 *
 * \param managed the managed pointer that will be converted.
 *
 * \return a managed pointer.
 */
template <typename Base, std::derived_from<Base> Derived>
[[nodiscard]] auto managed_cast(Managed<Derived> managed) noexcept -> Managed<Base>
{
  auto deleter = [](Base* ptr) noexcept { delete dynamic_cast<Derived*>(ptr); };
  return Managed<Base> {managed.release(), deleter};
}

}  // namespace tactile

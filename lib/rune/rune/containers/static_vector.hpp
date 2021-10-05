#ifndef RUNE_STATIC_VECTOR_HPP_
#define RUNE_STATIC_VECTOR_HPP_

#include <array>             // array
#include <cassert>           // assert
#include <concepts>          // default_initializable
#include <initializer_list>  // initializer_list
#include <utility>           // forward, move

#include "../common/integers.hpp"
#include "../core/rune_error.hpp"

namespace rune {

/// \addtogroup containers
/// \{

/**
 * \class static_vector
 *
 * \brief A sequence container with an API similar to that of `std::vector`, but the
 * elements are stored in a `std::array`, i.e. as a part of the object.
 *
 * \note This class performs no dynamic memory allocations by itself.
 *
 * \tparam T the element type.
 * \tparam Capacity the maximum capacity of the vector.
 */
template <std::default_initializable T, usize Capacity>
class static_vector final
{
 public:
  using size_type = usize;
  using value_type = T;
  using iterator = value_type*;
  using const_iterator = const value_type*;

  constexpr static_vector() = default;

  /**
   * \brief Creates a vector with the specified elements.
   *
   * \pre The size of the supplied list must not exceed the capacity of the vector.
   *
   * \details A terse way to use this constructor is using CTAD.
   * \code{cpp}
   *   static_vector vector = {1, 2, 3}; // decltype(vector) == static_vector<int, 3>
   * \endcode
   *
   * \param list the list of elements.
   */
  constexpr static_vector(std::initializer_list<value_type> list)
  {
    const auto size = list.size();
    assert(size <= Capacity);

    for (size_type index = 0u; auto value : list)
    {
      m_data.at(index) = std::move(value);
      ++index;
    }

    m_size = size;
  }

  /// Clears the vector.
  constexpr void clear() noexcept
  {
    m_size = 0;
  }

  /**
   * \brief Adds an element to the end of the vector.
   *
   * \param value the element that will be added.
   *
   * \throws rune_error if the vector is already full.
   */
  constexpr void push_back(const value_type& value)
  {
    const auto index = m_size;
    if (index < Capacity)
    {
      m_data.at(index) = value;
      ++m_size;
    }
    else
    {
      throw rune_error{"static_vector::push_back(): vector already full"};
    }
  }

  /// \copydoc push_back()
  constexpr void push_back(value_type&& value)
  {
    const auto index = m_size;
    if (index < Capacity)
    {
      m_data.at(index) = std::move(value);
      ++m_size;
    }
    else
    {
      throw rune_error{"static_vector::push_back(): vector already full"};
    }
  }

  /**
   * \brief Adds an element to the end of the vector, creating it from the supplied
   * arguments.
   *
   * \tparam Args the types of the arguments that will be forwarded.
   *
   * \param args the arguments that will be forwarded to an appropriate value constructor.
   *
   * \return a reference to the created element.
   *
   * \throws rune_error if the vector is already full.
   */
  template <typename... Args>
  constexpr auto emplace_back(Args&&... args) -> T&
  {
    const auto index = m_size;
    if (index < Capacity)
    {
      m_data.at(index) = value_type{std::forward<Args>(args)...};
      ++m_size;
      return m_data.at(index);
    }
    else
    {
      throw rune_error{"static_vector::emplace_back(): vector already full"};
    }
  }

  /**
   * \brief Removes the last element of the vector.
   *
   * \note No element is destroyed by this function, i.e. no destructor is called, the
   * size is simply decremented.
   *
   * \pre The vector must not be empty.
   */
  constexpr void pop_back() noexcept
  {
    assert(!empty() && "static_vector mustn't be empty when pop_back() is called");
    --m_size;
  }

  /**
   * \brief Serializes the vector.
   *
   * \param archive the archive that will be used to serialize the vector.
   */
  constexpr void serialize(auto& archive)
  {
    archive(m_data, m_size);
  }

  /**
   * \brief Returns the element at the specified index.
   *
   * \param index the index of the desired element.
   *
   * \return the element at the specified index.
   *
   * \throws rune_error if the index is out of bounds.
   */
  [[nodiscard]] constexpr auto at(const size_type index) -> value_type&
  {
    if (index < m_size)
    {
      return m_data.at(index);
    }
    else
    {
      throw rune_error{"static_vector::at(): invalid index"};
    }
  }

  /// \copydoc at()
  [[nodiscard]] constexpr auto at(const size_type index) const -> const value_type&
  {
    if (index < m_size)
    {
      return m_data.at(index);
    }
    else
    {
      throw rune_error{"static_vector::at(): invalid index"};
    }
  }

  /**
   * \brief Returns the element at the specified index.
   *
   * \warning This function performs no bounds checking.
   *
   * \pre `index` must not be out of bounds.
   *
   * \param index the index of the desired element.
   *
   * \return the element at the specified index.
   */
  [[nodiscard]] constexpr auto operator[](const size_type index) -> value_type&
  {
    assert(index < m_size);
    return m_data[index];
  }

  /// \copydoc operator[]()
  [[nodiscard]] constexpr auto operator[](const size_type index) const
      -> const value_type&
  {
    assert(index < m_size);
    return m_data[index];
  }

  /// \brief Returns the amount of elements.
  [[nodiscard]] constexpr auto size() const noexcept -> size_type
  {
    return m_size;
  }

  /// \brief Returns the maximum amount of elements.
  [[nodiscard]] constexpr auto capacity() const noexcept -> size_type
  {
    return Capacity;
  }

  /**
   * \brief Indicates whether or not the vector is empty.
   *
   * \return `true` if the vector is empty; `false` otherwise.
   */
  [[nodiscard]] constexpr auto empty() const noexcept -> bool
  {
    return m_size == 0;
  }

  /// \brief Returns an iterator to the beginning of the vector.
  [[nodiscard]] constexpr auto begin() noexcept -> iterator
  {
    return m_data.data();
  }

  /// \copydoc begin()
  [[nodiscard]] constexpr auto begin() const noexcept -> const_iterator
  {
    return m_data.data();
  }

  /// \brief Returns an iterator to the end of the vector.
  [[nodiscard]] constexpr auto end() noexcept -> iterator
  {
    return begin() + m_size;
  }

  /// \copydoc end()
  [[nodiscard]] constexpr auto end() const noexcept -> const_iterator
  {
    return begin() + m_size;
  }

 private:
  std::array<T, Capacity> m_data{};
  size_type m_size{};
};

template <typename T, typename... Rest>
static_vector(T, Rest...) -> static_vector<T, 1u + sizeof...(Rest)>;

/// \} End of group containers

}  // namespace rune

#endif  // RUNE_STATIC_VECTOR_HPP_

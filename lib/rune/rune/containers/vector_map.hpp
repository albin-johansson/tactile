#ifndef RUNE_VECTOR_MAP_HPP_
#define RUNE_VECTOR_MAP_HPP_

#include <algorithm>  // find_if
#include <cassert>    // assert
#include <concepts>   // convertible_to
#include <iterator>   // distance, iter_swap
#include <utility>    // pair, move, forward
#include <vector>     // vector

#include "../common/integers.hpp"
#include "../common/maybe.hpp"
#include "../core/rune_error.hpp"

namespace rune {

/// \addtogroup containers
/// \{

// clang-format off

template <typename T, typename U>
concept transparent_to = requires (T key, U actual)
{
  { key == actual } -> std::convertible_to<bool>;
  { actual == key } -> std::convertible_to<bool>;
};

// clang-format on

/**
 * \class vector_map
 *
 * \brief A cache-friendly associative container for small sets of key/value pairs.
 *
 * \details This container is useful when the number of pairs is small (approximately
 * 0-100 elements), due to the fact that the internal representation is a `std::vector`
 * of key/value pairs. This results in very fast iteration and lookup (as long as the
 * total amount of entries is relatively small) due to the contiguous layout of the
 * elements.
 *
 * \tparam K the key type.
 * \tparam V the value type.
 */
template <typename K, typename V>
class vector_map final
{
 public:
  using key_type = K;
  using mapped_type = V;
  using value_type = std::pair<key_type, mapped_type>;
  using size_type = usize;
  using iterator = typename std::vector<value_type>::iterator;
  using const_iterator = typename std::vector<value_type>::const_iterator;

  vector_map() noexcept = default;

  /**
   * \brief Creates a map with the specified storage capacity.
   *
   * \param capacity the amount of entries the map can store without needing needing to
   * allocate additional memory.
   */
  explicit vector_map(const size_type capacity)
  {
    reserve(capacity);
  }

  /// \brief Clears the map of all entries.
  void clear() noexcept
  {
    m_data.clear();
  }

  /**
   * \brief Ensures that the map can store the specified amount of entries without
   * reallocation.
   *
   * \note This function has no effect if the specified capacity isn't greater than the
   * current capacity.
   *
   * \param capacity the new capacity of the map.
   */
  void reserve(const size_type capacity)
  {
    m_data.reserve(capacity);
  }

  /**
   * \brief Adds a key/value pair to the map.
   *
   * \pre `key` must not be associated with an existing entry.
   *
   * \tparam Args the types of the arguments that will be forwarded.
   *
   * \param key the key that will be associated with the value.
   * \param args the arguments that will be forwarded to a `mapped_type` constructor.
   *
   * \return a reference to the added key/value pair.
   */
  template <typename... Args>
  auto emplace(key_type key, Args&&... args) -> value_type&
  {
    assert(!contains(key));
    return m_data.emplace_back(std::move(key), mapped_type{std::forward<Args>(args)...});
  }

  /**
   * \brief Adds a key/value pair to the map.
   *
   * \pre `key` must not be associated with an existing entry.
   *
   * \param key the key that will be associated with the value.
   * \param value the value that will be moved into the map.
   *
   * \return a reference to the added key/value pair.
   */
  auto emplace(key_type key, mapped_type value) -> value_type&
  {
    assert(!contains(key));
    return m_data.emplace_back(std::move(key), std::move(value));
  }

  /**
   * \brief Adds a key/value pair to the map, as long as there isn't already an entry
   * using the specified key.
   *
   * \tparam T the type of the key value.
   * \tparam Args the types of the arguments that will be forwarded.
   *
   * \param key the key that will be associated with the value.
   * \param args the arguments that will be forwarded to a `mapped_type` constructor.
   *
   * \return a reference to the added or existing key/value pair.
   */
  template <transparent_to<key_type> T, typename... Args>
  auto try_emplace(const T& key, Args&&... args) -> value_type&
  {
    if (const auto it = find(key); it != m_data.end())
    {
      return *it;
    }
    else
    {
      return emplace(key, std::forward<Args>(args)...);
    }
  }

  /**
   * \brief Adds or replaces a key/value pair in the map.
   *
   * \tparam Args the types of the arguments that will be forwarded.
   *
   * \param key the key that will be associated with the value.
   * \param args the arguments that will be forwarded to a `mapped_type` constructor.
   *
   * \return a reference to the added key/value pair.
   */
  template <typename... Args>
  auto emplace_or_replace(key_type key, Args&&... args) -> value_type&
  {
    if (const auto it = find(key); it != end())
    {
      it->second = mapped_type{std::forward<Args>(args)...};
      return *it;
    }
    else
    {
      return emplace(std::move(key), std::forward<Args>(args)...);
    }
  }

  /**
   * \brief Adds or replaces a key/value pair in the map.
   *
   * \param key the key that will be associated with the value.
   * \param value the value that will be moved into the map.
   *
   * \return a reference to the added key/value pair.
   */
  auto emplace_or_replace(key_type key, mapped_type value) -> value_type&
  {
    if (const auto it = find(key); it != end())
    {
      it->second = std::move(value);
      return *it;
    }
    else
    {
      return emplace(std::move(key), std::move(value));
    }
  }

  /**
   * \brief Removes an entry from the map.
   *
   * \note This function has no effect if there is no entry associated with the specified
   * key.
   *
   * \param key the key associated with the entry that will be removed.
   */
  template <transparent_to<key_type> T>
  void erase(const T& key)
  {
    std::erase_if(m_data, [&](const value_type& pair) { return pair.first == key; });
  }

  /**
   * \brief Moves the the specified key/value pair forwards in the underlying array by one
   * step.
   *
   * \param key the key/value pair that will be moved in the underlying vector.
   */
  template <transparent_to<key_type> T>
  void move_forward(const T& key)
  {
    move_element<true>(key);
  }

  /**
   * \brief Moves the the specified key/value pair backwards in the underlying array by
   * one step.
   *
   * \param key the key/value pair that will be moved in the underlying vector.
   */
  template <transparent_to<key_type> T>
  void move_backward(const T& key)
  {
    move_element<false>(key);
  }

  /**
   * \brief Returns an iterator to the element associated with the specified key.
   *
   * \tparam T the type of the key, must be comparable with the real key type.
   *
   * \param key the key to of the value to look for.
   *
   * \return an iterator to the found element; `end()` is returned if the element wasn't
   * found.
   */
  template <transparent_to<key_type> T>
  [[nodiscard]] auto find(const T& key) -> iterator
  {
    return std::find_if(m_data.begin(), m_data.end(), [&](const value_type& pair) {
      return pair.first == key;
    });
  }

  /// \copydoc find()
  template <transparent_to<key_type> T>
  [[nodiscard]] auto find(const T& key) const -> const_iterator
  {
    return std::find_if(m_data.begin(), m_data.end(), [&](const value_type& pair) {
      return pair.first == key;
    });
  }

  /**
   * \brief Returns the key/value pair at the specified index.
   *
   * \param index the index of the desired key/value pair.
   *
   * \throws std::out_of_range if the index is out out of bounds.
   *
   * \return a key/value pair.
   */
  [[nodiscard]] auto at_index(const size_type index) -> value_type&
  {
    return m_data.at(index);
  }

  /// \copydoc at_index()
  [[nodiscard]] auto at_index(const size_type index) const -> const value_type&
  {
    return m_data.at(index);
  }

  /**
   * \brief Returns the value associated with the specified key.
   *
   * \tparam T the type of the key, must be comparable with the real key type.
   *
   * \param key the key associated with the desired value.
   *
   * \return the value associated with the specified key.
   *
   * \throws rune_error if the key is not associated with a value.
   *
   * \since 0.1.0
   */
  template <transparent_to<key_type> T>
  [[nodiscard]] auto at(const T& key) -> mapped_type&
  {
    if (auto it = find(key); it != end())
    {
      return it->second;
    }
    else
    {
      throw rune_error{"vector_map::at(): invalid key"};
    }
  }

  /// \copydoc at()
  template <transparent_to<key_type> T>
  [[nodiscard]] auto at(const T& key) const -> const mapped_type&
  {
    if (const auto it = find(key); it != end())
    {
      return it->second;
    }
    else
    {
      throw rune_error{"vector_map::at(): invalid key"};
    }
  }

  /**
   * \brief Returns the existing value associated with the key, or attempts to insert a
   * default constructed value for the key.
   *
   * \tparam T the type of the key.
   *
   * \param key the key associated with the desired value.
   *
   * \return a reference to the existing or created value.
   */
  template <transparent_to<key_type> T>
  [[nodiscard]] auto operator[](const T& key) -> mapped_type&
  {
    return try_emplace(key).second;
  }

  /**
   * \brief Returns the index of the entry associated with the specified key.
   *
   * \param key the key of the entry to look for.
   *
   * \return the index of the specified entry in the underlying vector; `nothing` if
   * the key is unused.
   */
  template <transparent_to<key_type> T>
  [[nodiscard]] auto index_of(const T& key) const -> maybe<size_type>
  {
    if (const auto it = find(key); it != end())
    {
      return std::distance(begin(), it);
    }
    else
    {
      return nothing;
    }
  }

  /**
   * \brief Indicates whether or not the map contains the specified key.
   *
   * \param key the key to look for.
   *
   * \return `true` if the map contains the supplied key; `false` otherwise.
   */
  template <transparent_to<key_type> T>
  [[nodiscard]] auto contains(const T& key) const -> bool
  {
    return find(key) != end();
  }

  /**
   * \brief Indicates whether or not the map is empty.
   *
   * \return `true` if the map is empty; `false` otherwise.
   */
  [[nodiscard]] auto empty() const noexcept -> bool
  {
    return m_data.empty();
  }

  /// \brief Returns the amount of key/value pairs that are stored in the map.
  [[nodiscard]] auto size() const noexcept -> size_type
  {
    return m_data.size();
  }

  /// \brief Returns the amount of entries the map can store without re-allocation.
  [[nodiscard]] auto capacity() const noexcept -> size_type
  {
    return m_data.capacity();
  }

  /// \brief Returns an iterator to the beginning of the map.
  [[nodiscard]] auto begin() noexcept -> iterator
  {
    return m_data.begin();
  }

  /// \copydoc begin()
  [[nodiscard]] auto begin() const noexcept -> const_iterator
  {
    return m_data.begin();
  }

  /// \brief Returns an iterator to the end of the map.
  [[nodiscard]] auto end() noexcept -> iterator
  {
    return m_data.end();
  }

  /// \copydoc end()
  [[nodiscard]] auto end() const noexcept -> const_iterator
  {
    return m_data.end();
  }

 private:
  std::vector<value_type> m_data;

  template <bool MoveForward, transparent_to<key_type> T>
  void move_element(const T& key)
  {
    if (const auto it = find(key); it != end())
    {
      const auto index = std::distance(begin(), it);
      if constexpr (MoveForward)
      {
        if (index != size() - 1)
        {
          std::iter_swap(it, it + 1);
        }
      }
      else
      {
        if (index != 0)
        {
          std::iter_swap(it, it - 1);
        }
      }
    }
  }
};

/// \} End of group containers

}  // namespace rune

#endif  // RUNE_VECTOR_MAP_HPP_

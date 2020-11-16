#pragma once

#include <algorithm>  // swap_iter
#include <iterator>   // distance
#include <optional>   // optional
#include <ranges>     // find_if
#include <stdexcept>  // out_of_range
#include <utility>    // forward, pair, make_pair
#include <vector>     // vector

namespace tactile {

/**
 * \class vector_map
 *
 * \brief An associative container for small sets of key-value pairs.
 *
 * \details This container is useful when the number of pairs are small, often
 * less than 10 elements. This is because the internal representation is based
 * on a `std::vector`, which means that iteration and lookup is very fast and
 * cache-friendly.
 *
 * \tparam Key the type of the keys.
 * \tparam Value the type of the mapped values.
 *
 * \since 0.1.0
 *
 * \headerfile vector_map.hpp
 */
template <typename Key, typename Value>
class vector_map final
{
  using storage_type = std::vector<std::pair<Key, Value>>;

 public:
  using key_type = Key;
  using mapped_type = Value;
  using pair_type = std::pair<key_type, mapped_type>;
  using value_type = typename storage_type::value_type;
  using size_type = typename storage_type::size_type;
  using iterator = typename storage_type::iterator;
  using const_iterator = typename storage_type::const_iterator;

  /**
   * \brief Ensures that the map has capacity for the specified amount of
   * elements.
   *
   * \param capacity the amount of elements to reserve memory for.
   *
   * \since 0.1.0
   */
  void reserve(size_type capacity)
  {
    m_data.reserve(capacity);
  }

  /**
   * \brief Clears the map of all entries.
   *
   * \details The map is empty after this operation.
   *
   * \since 0.1.0
   */
  void clear() noexcept
  {
    m_data.clear();
  }

  /**
   * \brief Inserts a key/value-pair in-place.
   *
   * \details This function will replace any previous existing key/value-pair
   * that uses the specified key.
   *
   * \tparam Args the types of the arguments that will be forwarded.
   *
   * \param key the key that will be associated with the value.
   * \param args the arguments that will be forwarded to the appropriate value
   * constructor.
   *
   * \return a reference to the inserted key/value-pair.
   *
   * \since 0.1.0
   */
  template <typename... Args>
  decltype(auto) emplace(const key_type& key, Args&&... args)
  {
    if (const auto it = find(key); it != m_data.end()) {
      m_data.erase(it);
    }
    return m_data.emplace_back(key, mapped_type{std::forward<Args>(args)...});
  }

  /**
   * \brief Inserts a key/value-pair.
   *
   * \details This function will replace any previous existing key/value-pair
   * that uses the specified key.
   *
   * \param key the key that will be associated with the value.
   * \param value the value that will be associated with the key.
   *
   * \return a reference to the inserted key/value-pair.
   *
   * \since 0.1.0
   */
  decltype(auto) emplace(const key_type& key, mapped_type&& value)
  {
    if (const auto it = find(key); it != m_data.end()) {
      m_data.erase(it);
    }
    return m_data.emplace_back(key, std::move(value));
  }

  /**
   * \brief Removes the key/value-pair associated with the specified key.
   *
   * \param key the key of the pair that will be removed.
   *
   * \since 0.1.0
   */
  void erase(const key_type& key)
  {
    if (const auto it = find(key); it != m_data.end()) {
      m_data.erase(it);
    }
  }

  [[nodiscard]] auto at_index(std::size_t index) -> pair_type&
  {
    return m_data.at(index);
  }

  [[nodiscard]] auto at_index(std::size_t index) const -> const pair_type&
  {
    return m_data.at(index);
  }

  /**
   * \brief Returns the value associated with the specified key.
   *
   * \param key the key associated with the desired value.
   *
   * \return the value associated with the specified key.
   *
   * \throws out_of_range if the key is not associated with a value.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto at(const key_type& key) -> mapped_type&
  {
    if (auto it = find(key); it != m_data.end()) {
      return it->second;
    } else {
      throw std::out_of_range{"Couldn't find key!"};
    }
  }

  /**
   * \copydoc at(const key_type&)
   */
  [[nodiscard]] auto at(const key_type& key) const -> const mapped_type&
  {
    if (const auto it = find(key); it != m_data.end()) {
      return it->second;
    } else {
      throw std::out_of_range{"Couldn't find key!"};
    }
  }

  /**
   * \copydoc at(const key_type&)
   */
  [[nodiscard]] auto operator[](const key_type& key) -> mapped_type&
  {
    return at(key);
  }

  /**
   * \copydoc at(const key_type&)
   */
  [[nodiscard]] auto operator[](const key_type& key) const -> const mapped_type&
  {
    return at(key);
  }

  /**
   * \brief Returns an iterator to the element associated with the specified
   * key.
   *
   * \param key the key to lookup.
   *
   * \return an iterator to the found element; `end()` is returned if the
   * element wasn't found.
   */
  [[nodiscard]] auto find(const key_type& key) -> iterator
  {
    return std::ranges::find_if(m_data, [key](const auto& pair) {
      return pair.first == key;
    });
  }

  /**
   * \copydoc find
   */
  [[nodiscard]] auto find(const key_type& key) const -> const_iterator
  {
    return std::ranges::find_if(m_data, [key](const auto& pair) {
      return pair.first == key;
    });
  }

  /**
   * \brief Indicates whether or not the map contains the specified key.
   *
   * \param key the key to look for.
   *
   * \return `true` if the map contains the supplied key; `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto contains(const key_type& key) const -> bool
  {
    return find(key) != m_data.end();
  }

  /**
   * \brief Returns the index of the key/value-pair associated with the
   * specified key in the underlying vector.
   *
   * \param key the key of the pair to query.
   *
   * \return the index of the pair in the underlying vector; `std::nullopt` if
   * the key is unknown.
   */
  [[nodiscard]] auto index_of(const key_type& key) const
      -> std::optional<std::size_t>
  {
    if (const auto it = find(key); it != end()) {
      return std::distance(begin(), it);
    } else {
      return std::nullopt;
    }
  }

  void move_elem_forward(const key_type& key)
  {
    move_elem(key, true);
  }

  void move_elem_back(const key_type& key)
  {
    move_elem(key, false);
  }

  [[nodiscard]] auto front() -> pair_type&
  {
    return m_data.front();
  }

  [[nodiscard]] auto front() const -> const pair_type&
  {
    return m_data.front();
  }

  [[nodiscard]] auto back() -> pair_type&
  {
    return m_data.back();
  }

  [[nodiscard]] auto back() const -> const pair_type&
  {
    return m_data.front();
  }

  /**
   * \brief Indicates whether or not the map is empty.
   *
   * \return `true` if the map is empty; `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto empty() const noexcept -> bool
  {
    return m_data.empty();
  }

  /**
   * \brief Returns the number of elements stored in the map.
   *
   * \return the number of elements stored in the map.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto size() const noexcept -> size_type
  {
    return m_data.size();
  }

  /**
   * \brief Returns the capacity of the map.
   *
   * \details The capacity is the number of key/value-pairs the map can store
   * before having to re-allocate memory.
   *
   * \return the current capacity of the map.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto capacity() const noexcept -> size_type
  {
    return m_data.capacity();
  }

  /**
   * \brief Returns an iterator to the first element.
   *
   * \return an iterator to the first element.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto begin() noexcept -> iterator
  {
    return m_data.begin();
  }

  /**
   * \copydoc begin()
   */
  [[nodiscard]] auto begin() const noexcept -> const_iterator
  {
    return m_data.begin();
  }

  /**
   * \brief Returns an iterator one-past the last element.
   *
   * \return an iterator one-past the last element.
   */
  [[nodiscard]] auto end() noexcept -> iterator
  {
    return m_data.end();
  }

  /**
   * \copydoc end()
   */
  [[nodiscard]] auto end() const noexcept -> const_iterator
  {
    return m_data.end();
  }

 private:
  storage_type m_data;

  void move_elem(const key_type& key, bool forward)
  {
    if (const auto it = find(key); it != end()) {
      const auto index = std::distance(begin(), it);
      if (forward) {
        if (index != 0) {
          std::iter_swap(it, it - 1);
        }
      } else {
        if (index != size() - 1) {
          std::iter_swap(it, it + 1);
        }
      }
    }
  }
};

}  // namespace tactile

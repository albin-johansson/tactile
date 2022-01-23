#pragma once

#include <algorithm>  // find_if, erase_if, iter_swap
#include <iterator>   // distance
#include <utility>    // pair, move, forward
#include <vector>     // vector

#include <tactile_def.hpp>
#include <tactile_stdlib.hpp>

#include "throw.hpp"

namespace tactile {

template <typename K, typename V>
class vector_map final {
 public:
  using key_type = K;
  using mapped_type = V;
  using value_type = std::pair<key_type, mapped_type>;
  using storage_type = std::vector<value_type>;
  using iterator = typename storage_type::iterator;
  using const_iterator = typename storage_type::const_iterator;
  using size_type = usize;

  explicit vector_map(const size_type n) { reserve(n); }

  vector_map() = default;

  vector_map(const vector_map&) = default;
  auto operator=(const vector_map&) -> vector_map& = default;

  vector_map(vector_map&&) noexcept = default;
  auto operator=(vector_map&&) noexcept -> vector_map& = default;

  ~vector_map() noexcept = default;

  void clear() { mData.clear(); }

  void reserve(const size_type n) { mData.reserve(n); }

  template <typename... Args>
  auto emplace(key_type key, Args&&... args) -> value_type&
  {
    if (!contains(key)) {
      return mData.emplace_back(std::move(key), mapped_type{std::forward<Args>(args)...});
    }
    else {
      ThrowTraced(TactileError{"Duplicate key!"});
    }
  }

  template <typename... Args>
  auto emplace_or_replace(key_type key, Args&&... args) -> value_type&
  {
    if (const auto it = find(key); it != end()) {
      it->second = mapped_type{std::forward<Args>(args)...};
      return *it;
    }
    else {
      return emplace(std::move(key), std::forward<Args>(args)...);
    }
  }

  template <typename... Args>
  auto try_emplace(const key_type& key, Args&&... args) -> value_type&
  {
    if (const auto it = find(key); it != end()) {
      it->second = mapped_type{std::forward<Args>(args)...};
      return *it;
    }
    else {
      return emplace(key, std::forward<Args>(args)...);
    }
  }

  template <typename T>
  void erase(const T& key)
  {
    std::erase_if(mData, [&key](const value_type& pair) { return pair.first == key; });
  }

  template <typename T>
  void move_later(const T& key)
  {
    move_elem<true>(key);
  }

  template <typename T>
  void move_earlier(const T& key)
  {
    move_elem<false>(key);
  }

  template <typename T>
  [[nodiscard]] auto find(const T& key) -> iterator
  {
    return std::find_if(mData.begin(), mData.end(), [&key](const value_type& pair) {
      return pair.first == key;
    });
  }

  template <typename T>
  [[nodiscard]] auto find(const T& key) const -> const_iterator
  {
    return std::find_if(mData.begin(), mData.end(), [&key](const value_type& pair) {
      return pair.first == key;
    });
  }

  template <typename T>
  [[nodiscard]] auto operator[](const T& key) -> mapped_type&
  {
    return try_emplace(key).second;
  }

  template <typename T>
  [[nodiscard]] auto at(const T& key) -> mapped_type&
  {
    if (auto it = find(key); it != end()) {
      return it->second;
    }
    else {
      ThrowTraced(TactileError{"Invalid key!"});
    }
  }

  template <typename T>
  [[nodiscard]] auto at(const T& key) const -> const mapped_type&
  {
    if (auto it = find(key); it != end()) {
      return it->second;
    }
    else {
      ThrowTraced(TactileError{"Invalid key!"});
    }
  }

  [[nodiscard]] auto at_index(const usize index) -> value_type&
  {
    if (index < size()) {
      return mData.at(index);
    }
    else {
      ThrowTraced(TactileError{"Invalid index!"});
    }
  }

  [[nodiscard]] auto at_index(const usize index) const -> const value_type&
  {
    if (index < size()) {
      return mData.at(index);
    }
    else {
      ThrowTraced(TactileError{"Invalid index!"});
    }
  }

  template <typename T>
  [[nodiscard]] auto contains(const T& key) const -> bool
  {
    return find(key) != end();
  }

  template <typename T>
  [[nodiscard]] auto index_of(const T& key) const -> Maybe<size_type>
  {
    if (const auto it = find(key); it != end()) {
      return std::distance(begin(), it);
    }
    else {
      return nothing;
    }
  }

  [[nodiscard]] auto size() const noexcept -> size_type { return mData.size(); }

  [[nodiscard]] auto capacity() const noexcept -> size_type { return mData.capacity(); }

  [[nodiscard]] auto empty() const noexcept -> bool { return mData.empty(); }

  [[nodiscard]] auto begin() noexcept -> iterator { return mData.begin(); }

  [[nodiscard]] auto end() noexcept -> iterator { return mData.end(); }

  [[nodiscard]] auto begin() const noexcept -> const_iterator { return mData.begin(); }

  [[nodiscard]] auto end() const noexcept -> const_iterator { return mData.end(); }

 private:
  storage_type mData;

  template <bool MoveLater, typename T>
  void move_elem(const T& key)
  {
    if (const auto it = find(key); it != end()) {
      const auto index = std::distance(begin(), it);
      if constexpr (MoveLater) {
        if (static_cast<usize>(index) != size() - 1u) {
          std::iter_swap(it, it + 1);
        }
      }
      else {
        if (index != 0) {
          std::iter_swap(it, it - 1);
        }
      }
    }
  }
};

}  // namespace tactile

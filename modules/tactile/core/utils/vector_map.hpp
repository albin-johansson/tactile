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
class VectorMap final {
 public:
  using key_type = K;
  using mapped_type = V;
  using value_type = std::pair<key_type, mapped_type>;
  using storage_type = std::vector<value_type>;
  using iterator = typename storage_type::iterator;
  using const_iterator = typename storage_type::const_iterator;
  using size_type = usize;

  explicit VectorMap(const size_type n) { Reserve(n); }

  VectorMap() = default;

  VectorMap(const VectorMap&) = default;
  auto operator=(const VectorMap&) -> VectorMap& = default;

  VectorMap(VectorMap&&) noexcept = default;
  auto operator=(VectorMap&&) noexcept -> VectorMap& = default;

  ~VectorMap() noexcept = default;

  void Clear() { mData.clear(); }

  void Reserve(const size_type n) { mData.reserve(n); }

  template <typename... Args>
  auto Emplace(key_type key, Args&&... args) -> value_type&
  {
    if (!Contains(key)) {
      return mData.emplace_back(std::move(key), mapped_type{std::forward<Args>(args)...});
    }
    else {
      ThrowTraced(TactileError{"Duplicate key!"});
    }
  }

  template <typename... Args>
  auto EmplaceOrReplace(key_type key, Args&&... args) -> value_type&
  {
    if (const auto it = Find(key); it != end()) {
      it->second = mapped_type{std::forward<Args>(args)...};
      return *it;
    }
    else {
      return Emplace(std::move(key), std::forward<Args>(args)...);
    }
  }

  template <typename... Args>
  auto TryEmplace(const key_type& key, Args&&... args) -> value_type&
  {
    if (const auto it = Find(key); it != end()) {
      it->second = mapped_type{std::forward<Args>(args)...};
      return *it;
    }
    else {
      return Emplace(key, std::forward<Args>(args)...);
    }
  }

  template <typename T>
  void Erase(const T& key)
  {
    std::erase_if(mData, [&key](const value_type& pair) { return pair.first == key; });
  }

  template <typename T>
  void MoveLater(const T& key)
  {
    MoveElement<true>(key);
  }

  template <typename T>
  void MoveEarlier(const T& key)
  {
    MoveElement<false>(key);
  }

  template <typename T>
  [[nodiscard]] auto Find(const T& key) -> iterator
  {
    return std::find_if(mData.begin(), mData.end(), [&key](const value_type& pair) {
      return pair.first == key;
    });
  }

  template <typename T>
  [[nodiscard]] auto Find(const T& key) const -> const_iterator
  {
    return std::find_if(mData.begin(), mData.end(), [&key](const value_type& pair) {
      return pair.first == key;
    });
  }

  template <typename T>
  [[nodiscard]] auto operator[](const T& key) -> mapped_type&
  {
    return TryEmplace(key).second;
  }

  template <typename T>
  [[nodiscard]] auto At(const T& key) -> mapped_type&
  {
    if (auto it = Find(key); it != end()) {
      return it->second;
    }
    else {
      ThrowTraced(TactileError{"Invalid key!"});
    }
  }

  template <typename T>
  [[nodiscard]] auto At(const T& key) const -> const mapped_type&
  {
    if (auto it = Find(key); it != end()) {
      return it->second;
    }
    else {
      ThrowTraced(TactileError{"Invalid key!"});
    }
  }

  [[nodiscard]] auto AtIndex(const usize index) -> value_type&
  {
    if (index < GetSize()) {
      return mData.at(index);
    }
    else {
      ThrowTraced(TactileError{"Invalid index!"});
    }
  }

  [[nodiscard]] auto AtIndex(const usize index) const -> const value_type&
  {
    if (index < GetSize()) {
      return mData.at(index);
    }
    else {
      ThrowTraced(TactileError{"Invalid index!"});
    }
  }

  template <typename T>
  [[nodiscard]] auto Contains(const T& key) const -> bool
  {
    return Find(key) != end();
  }

  template <typename T>
  [[nodiscard]] auto IndexOf(const T& key) const -> Maybe<size_type>
  {
    if (const auto it = Find(key); it != end()) {
      return std::distance(begin(), it);
    }
    else {
      return nothing;
    }
  }

  [[nodiscard]] auto GetSize() const noexcept -> size_type { return mData.size(); }

  [[nodiscard]] auto GetCapacity() const noexcept -> size_type
  {
    return mData.capacity();
  }

  [[nodiscard]] auto IsEmpty() const noexcept -> bool { return mData.empty(); }

  [[nodiscard]] auto begin() noexcept -> iterator { return mData.begin(); }

  [[nodiscard]] auto end() noexcept -> iterator { return mData.end(); }

  [[nodiscard]] auto begin() const noexcept -> const_iterator { return mData.begin(); }

  [[nodiscard]] auto end() const noexcept -> const_iterator { return mData.end(); }

 private:
  storage_type mData;

  template <bool MoveLater, typename T>
  void MoveElement(const T& key)
  {
    if (const auto it = Find(key); it != end()) {
      const auto index = std::distance(begin(), it);
      if constexpr (MoveLater) {
        if (index != GetSize() - 1) {
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

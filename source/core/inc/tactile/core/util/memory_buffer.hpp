// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <algorithm>  // min, copy_n
#include <concepts>   // same_as, input_iterator
#include <cstdlib>    // abs
#include <iterator>   // distance

#include "tactile/base/container/array.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/numeric/saturate_cast.hpp"
#include "tactile/base/numeric/sign_cast.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile {

inline constexpr usize kDefaultMemoryBufferCapacity = 512;

/**
 * Represents a fixed-size buffer.
 *
 * \tparam T        The type of the underlying buffer elements.
 * \tparam Capacity The maximum number of stored elements.
 */
template <typename T = uint8, usize Capacity = kDefaultMemoryBufferCapacity>
  requires(Capacity > 0)
class MemoryBuffer
{
 public:
  using value_type = T;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using iterator = pointer;
  using const_iterator = const_pointer;
  using size_type = usize;

  /**
   * Creates an empty buffer.
   */
  constexpr MemoryBuffer() noexcept = default;

  constexpr ~MemoryBuffer() noexcept = default;

  TACTILE_DELETE_COPY(MemoryBuffer);
  TACTILE_DELETE_MOVE(MemoryBuffer);

  /**
   * Resets the buffer size to zero.
   *
   * \note
   * For efficiency reasons, this function doesn't actually clear the underlying
   * buffer content, it just resets the internal pointers.
   */
  constexpr void clear() noexcept
  {
    mBegin = mBuf.data();
    mEnd = mBegin;
  }

  /**
   * Adds an value to the buffer.
   *
   * \details
   * This function has no effect if there is no room left in the buffer.
   *
   * \param value The value to add.
   */
  constexpr void push_back(const value_type value) noexcept
  {
    const auto curr_size = size();
    if (curr_size < capacity()) {
      *mEnd = value;
      ++mEnd;
    }
  }

  /**
   * Appends a range of elements to the buffer.
   *
   * \details
   * Only the elements that fit in the remaining buffer space will be included.
   *
   * \param begin The beginning of the elements to add.
   * \param end   The end of the elements to add.
   */
  template <std::input_iterator Iter>
  constexpr void append(const Iter begin, const Iter end) noexcept
  {
    const auto avail_count = remaining_capacity();
    const auto elem_count = to_unsigned(std::abs(std::distance(begin, end)));
    const auto n = std::min(avail_count, elem_count);
    mEnd = std::copy_n(begin, n, mEnd);
  }

  /**
   * Appends the elements in a container to the buffer.
   *
   * \details
   * Only the elements that fit in the remaining buffer space will be included.
   *
   * \param container The source container.
   */
  template <typename Container>
    requires(std::same_as<typename Container::value_type, value_type>)
  constexpr void append(const Container& container) noexcept
  {
    append(container.begin(), container.end());
  }

  /**
   * Returns the elements at the specified position.
   *
   * \pre The index must refer to a valid element in the buffer.
   *
   * \param index The element index.
   *
   * \return
   * The element at the given index.
   */
  [[nodiscard]]
  constexpr auto operator[](const size_type index) noexcept -> reference
  {
    TACTILE_ASSERT(index < size());
    return mBuf[index];
  }

  /**
   * \copydoc operator[]()
   */
  [[nodiscard]]
  constexpr auto operator[](const size_type index) const noexcept
      -> const_reference
  {
    TACTILE_ASSERT(index < size());
    return mBuf[index];
  }

  /**
   * Returns the elements at the specified position.
   *
   * \details
   * An exception is thrown if the provided index is invalid.
   *
   * \param index The element index.
   *
   * \return
   * The element at the given index.
   */
  [[nodiscard]]
  constexpr auto at(const size_type index) -> reference
  {
    if (index >= size()) [[unlikely]] {
      throw Exception {"bad index"};
    }

    return mBuf[index];
  }

  /**
   * \copydoc at()
   */
  [[nodiscard]]
  constexpr auto at(const size_type index) const -> const_reference
  {
    if (index >= size()) [[unlikely]] {
      throw Exception {"bad index"};
    }

    return mBuf[index];
  }

  /**
   * Returns a pointer to the internal array.
   *
   * \return
   * A pointer to the underlying data. A null pointer is returned if the buffer
   * is empty.
   */
  [[nodiscard]]
  constexpr auto data() noexcept -> pointer
  {
    return empty() ? nullptr : mBuf.data();
  }

  /**
   * \copydoc data()
   */
  [[nodiscard]]
  constexpr auto data() const noexcept -> const_pointer
  {
    return empty() ? nullptr : mBuf.data();
  }

  /**
   * Returns an iterator to the beginning of the buffer data.
   *
   * \return
   * An iterator.
   */
  [[nodiscard]]
  constexpr auto begin() noexcept -> iterator
  {
    TACTILE_ASSERT(mBegin != nullptr);
    return mBegin;
  }

  /**
   * \copydoc begin()
   */
  [[nodiscard]]
  constexpr auto begin() const noexcept -> const_iterator
  {
    TACTILE_ASSERT(mBegin != nullptr);
    return mBegin;
  }

  /**
   * Returns an iterator to the end of the buffer data.
   *
   * \return
   * An iterator.
   */
  [[nodiscard]]
  constexpr auto end() noexcept -> iterator
  {
    TACTILE_ASSERT(mBegin != nullptr);
    return mEnd;
  }

  /**
   * \copydoc end()
   */
  [[nodiscard]]
  constexpr auto end() const noexcept -> const_iterator
  {
    TACTILE_ASSERT(mBegin != nullptr);
    return mEnd;
  }

  /**
   * Returns the number of elements in the buffer.
   *
   * \return
   * The number of elements.
   */
  [[nodiscard]]
  constexpr auto size() const noexcept -> size_type
  {
    return saturate_cast<size_type>(std::distance(mBegin, mEnd));
  }

  /**
   * Returns the remaining number elements that could be stored in the buffer.
   *
   * \return
   * The number of elements.
   */
  [[nodiscard]]
  constexpr auto remaining_capacity() const noexcept -> size_type
  {
    TACTILE_ASSERT(size() <= Capacity);
    return Capacity - size();
  }

  /**
   * Returns the maximum number elements that can be stored in the buffer.
   *
   * \return
   * The maximum number of elements.
   */
  [[nodiscard]]
  constexpr auto capacity() const noexcept -> size_type
  {
    return Capacity;
  }

  /**
   * Indicates whether the buffer is full.
   *
   * \return
   * True if the buffer is full; false otherwise.
   */
  [[nodiscard]]
  constexpr auto full() const noexcept -> bool
  {
    return size() == capacity();
  }

  /**
   * Indicates whether the buffer is empty.
   *
   * \return
   * True if the buffer is empty; false otherwise.
   */
  [[nodiscard]]
  constexpr auto empty() const noexcept -> bool
  {
    return size() == 0;
  }

  /**
   * Returns a string view into the buffer.
   *
   * \return
   * A string view.
   */
  [[nodiscard]]
  constexpr auto view() const noexcept -> StringView
    requires(std::same_as<value_type, StringView::value_type>)
  {
    return StringView {data(), size()};
  }

 private:
  // The buffer is not initialized by default, to avoid expensive dead writes.
  Array<value_type, Capacity> mBuf;
  iterator mBegin {mBuf.data()};
  iterator mEnd {mBegin};
};

template class MemoryBuffer<char>;
template class MemoryBuffer<uint8>;

}  // namespace tactile

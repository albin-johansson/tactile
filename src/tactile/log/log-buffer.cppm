// Copyright (C) 2025 Albin Johansson

export module tactile.log:buffer;

export import tactile.core;

namespace tactile {

template <usize N>
class LogBuffer final
{
 public:
  using value_type = char;
  using iterator = char*;
  using const_iterator = const char*;

  LogBuffer() noexcept = default;

  LogBuffer(LogBuffer&&) = delete;

  LogBuffer(const LogBuffer&) = delete;

  ~LogBuffer() noexcept = default;

  auto operator=(LogBuffer&&) -> LogBuffer& = delete;

  auto operator=(const LogBuffer&) -> LogBuffer& = delete;

  void clear() noexcept
  {
    m_size = 0;
  }

  void push_back(const char ch) noexcept
  {
    if (m_size < N) {
      m_data[m_size] = ch;
      ++m_size;
    }
  }

  [[nodiscard]]
  auto view() const noexcept -> StringView
  {
    return StringView {m_data.data(), m_size};
  }

  [[nodiscard]]
  auto begin() noexcept -> iterator
  {
    return m_data.data();
  }

  [[nodiscard]]
  auto begin() const noexcept -> const_iterator
  {
    return m_data.data();
  }

  [[nodiscard]]
  auto end() noexcept -> iterator
  {
    return begin() + m_size;
  }

  [[nodiscard]]
  auto end() const noexcept -> const_iterator
  {
    return begin() + m_size;
  }

 private:
  Array<char, N> m_data {};
  usize m_size {};
};

}  // namespace tactile

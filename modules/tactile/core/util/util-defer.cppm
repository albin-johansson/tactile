// Copyright (C) 2025 Albin Johansson

export module tactile.core.util:defer;

export import tactile.core.ext.std;
export import tactile.core.log;

export namespace tactile {

/// Utility for creating inline destructors.
template <std::invocable T>
class Defer final
{
 public:
  [[nodiscard]]
  explicit Defer(T callback) noexcept
    : m_callback {std::move(callback)}
  {}

  Defer(Defer&&) = delete;

  Defer(const Defer&) = delete;

  ~Defer() noexcept
  {
    try {
      m_callback();
    }
    catch (const std::exception& error) {
      get_logger().log(LogLevel::kError,
                       "Defer destructor threw exception: {}",
                       error.what());
    }
    catch (...) {
      get_logger().log(LogLevel::kError, "Defer destructor threw exception");
    }
  }

  auto operator=(Defer&&) -> Defer& = delete;

  auto operator=(const Defer&) -> Defer& = delete;

 private:
  T m_callback;
};

}  // namespace tactile

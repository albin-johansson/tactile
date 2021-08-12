#ifndef RUNE_CORE_SEMI_FIXED_GAME_LOOP_HPP
#define RUNE_CORE_SEMI_FIXED_GAME_LOOP_HPP

#include <centurion.hpp>  // ...
#include <concepts>       // floating_point, derived_from

#include "../math/min.hpp"
#include "game.hpp"
#include "rune_error.hpp"

namespace rune {

/// \addtogroup core
/// \{

/// \name Configuration macros
/// \{

/**
 * \def RUNE_MAX_TICK_RATE
 *
 * \brief The maximum tick rate of the game loop, i.e. the maximum amount of ticks per
 * second.
 *
 * \details The game loop will try to run at the refresh rate of the primary screen, as
 * long as the the refresh rate isn't greater than the value of this macro. By default,
 * this macro expands to `120.0`.
 *
 * \note The value of this macro should be a `double`.
 */
#ifndef RUNE_MAX_TICK_RATE
#define RUNE_MAX_TICK_RATE 120.0
#endif  // RUNE_MAX_TICK_RATE

/**
 * \def RUNE_ENGINE_MAX_FRAMES_PER_TICK
 *
 * \brief The maximum amount of frames that the game loop can run per tick.
 *
 * \details The purpose of this limit is to avoid the "spiral-of-death". By default, this
 * macro expands to `5`.
 *
 * \note The value of this macro should be an `int`.
 */
#ifndef RUNE_ENGINE_MAX_FRAMES_PER_TICK
#define RUNE_ENGINE_MAX_FRAMES_PER_TICK 5
#endif  // RUNE_ENGINE_MAX_FRAMES_PER_TICK

/// \} End of configuration macros

/// \copybrief RUNE_MAX_TICK_RATE
/// \see `RUNE_MAX_TICK_RATE`
inline constexpr double max_tick_rate = RUNE_MAX_TICK_RATE;

/// \copybrief RUNE_ENGINE_MAX_FRAMES_PER_TICK
/// \see `RUNE_ENGINE_MAX_FRAMES_PER_TICK`
inline constexpr int engine_max_frames_per_tick = RUNE_ENGINE_MAX_FRAMES_PER_TICK;

/**
 * \brief Returns the tick rate used by the game loop.
 *
 * \details The tick rate is determined by comparing the refresh rate of the primary
 * screen and the maximum tick rate, and selecting the minimum value.
 *
 * \return the tick rate used by the game loop.
 *
 * \see `max_tick_rate`
 * \see `RUNE_MAX_TICK_RATE`
 */
[[nodiscard]] inline auto tick_rate() -> double
{
  return min(max_tick_rate, static_cast<double>(cen::screen::refresh_rate().value()));
}

template <typename Game, typename Graphics>
class engine;

/**
 * \class semi_fixed_game_loop
 *
 * \brief Represents a "semi-fixed" game loop, that strives to use a fixed delta, but it
 * can be adjusted dynamically for a few frames at a time.
 *
 * \details The game loop will use a tick rate that depends on the refresh rate of the
 * current monitor, but the tick rate is limited to be at most `max_tick_rate`, see
 * `tick_rate()` for more information.
 *
 * \tparam Game the game type.
 * \tparam Graphics the graphics context type.
 *
 * \see `tick_rate()`
 * \see `max_tick_rate`
 * \see `engine_max_frames_per_tick`
 *
 * \since 0.1.0
 */
template <typename Game, typename Graphics>
class semi_fixed_game_loop
{
 public:
  using game_type = Game;
  using graphics_type = Graphics;
  using engine_type = engine<game_type, graphics_type>;
  using seconds_type = cen::seconds<double>;

  explicit semi_fixed_game_loop(engine_type* engine)
      : m_engine{engine}
      , m_rate{tick_rate()}
      , m_delta{1.0 / m_rate}
      , m_current{cen::counter::now_in_seconds<double>()}
  {
    if (!m_engine)
    {
      throw rune_error{"Cannot create semi_fixed_game_loop from null engine!"};
    }
  }

  void fetch_current_time() noexcept
  {
    m_current = cen::counter::now_in_seconds<double>();
  }

  void tick()
  {
    const auto newTime = cen::counter::now_in_seconds<double>();
    auto frameTime = newTime - m_current;

    m_current = newTime;
    auto nSteps = 0;

    while (frameTime > seconds_type::zero())
    {
      if (nSteps > engine_max_frames_per_tick)
      {
        break;  // avoids spiral-of-death by limiting maximum amount of steps
      }

      m_running = m_engine->update_input();
      if (!m_running)
      {
        break;
      }

      const auto dt = rune::min(frameTime, m_delta);
      m_engine->update_logic(static_cast<float>(dt.count()));

      frameTime -= dt;

      ++nSteps;
    }
  }

  [[nodiscard]] auto is_running() const noexcept -> bool
  {
    return m_running;
  }

 private:
  engine_type* m_engine{};
  double m_rate;
  seconds_type m_delta;
  seconds_type m_current;
  bool m_running{true};
};

/// \} End of group core

}  // namespace rune

#endif  // RUNE_CORE_SEMI_FIXED_GAME_LOOP_HPP

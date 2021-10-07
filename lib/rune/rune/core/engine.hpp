#ifndef RUNE_ENGINE_HPP_
#define RUNE_ENGINE_HPP_

#include <algorithm>      // min, max
#include <cassert>        // assert
#include <centurion.hpp>  // window, event, counter, screen, keyboard, mouse
#include <concepts>       // invocable
#include <entt.hpp>       // registry, dispatcher, delegate
#include <utility>        // move
#include <vector>         // vector

#include "../common/maybe.hpp"
#include "configuration.hpp"

namespace rune {

/// \addtogroup core
/// \{

class engine final
{
 public:
  using seconds_type = cen::seconds<double>;
  using delta_type = float;

  explicit engine(const configuration& cfg = {})
      : mWindow{cfg.window_title, cfg.window_size}
      , mRate{std::min(
            cfg.max_tick_rate,
            static_cast<seconds_type::rep>(cen::screen::refresh_rate().value()))}
      , mMaxFramesPerTick{std::max(cfg.max_frames_per_tick, 1)}
      , mDelta{1.0 / mRate}
      , mCurrent{cen::counter::now_in_seconds<seconds_type::rep>()}
  {
    mInputSystems.reserve(4u);
    mLogicSystems.reserve(32u);
    mRenderSystems.reserve(32u);
  }

  /**
   * \brief Starts the game loop and runs the game.
   *
   * \return always `0`.
   */
  auto run() -> int
  {
    mWindow.show();
    mCurrent = cen::counter::now_in_seconds<seconds_type::rep>();

    if (mOnStart)
    {
      mOnStart();
    }

    while (mRunning)
    {
      tick();
      render();
    }

    if (mOnExit)
    {
      mOnExit();
    }

    mWindow.hide();
    return 0;
  }

  void stop() noexcept
  {
    mRunning = false;
  }

  template <auto Func>
  void on_start()
  {
    mOnStart.template connect<Func>();
  }

  template <auto Func, typename T>
  void on_start(T* self)
  {
    mOnStart.template connect<Func>(self);
  }

  template <std::invocable T>
  void on_start(T func)
  {
    mOnStart = std::move(func);
  }

  template <auto Func>
  void on_exit()
  {
    mOnExit.template connect<Func>();
  }

  template <auto Func, typename T>
  void on_exit(T* self)
  {
    mOnExit.template connect<Func>(self);
  }

  template <std::invocable T>
  void on_exit(T func)
  {
    mOnExit = std::move(func);
  }

  template <auto Func>
  void add_logic_system()
  {
    auto& delegate = mLogicSystems.emplace_back();
    delegate.template connect<Func>();
  }

  template <auto Func, typename T>
  void add_logic_system(T* self)
  {
    auto& delegate = mLogicSystems.emplace_back();
    delegate.template connect<Func>(self);
  }

  template <std::invocable<entt::registry&, entt::dispatcher&, delta_type> T>
  void add_logic_system(T func)
  {
    auto& delegate = mLogicSystems.emplace_back();
    delegate = std::move(func);
  }

  template <auto Func>
  void add_input_system()
  {
    auto& delegate = mInputSystems.emplace_back();
    delegate.template connect<Func>();
  }

  template <auto Func, typename T>
  void add_input_system(T* self)
  {
    auto& delegate = mInputSystems.emplace_back();
    delegate.template connect<Func>(self);
  }

  template <std::invocable<entt::registry&, entt::dispatcher&> T>
  void add_input_system(T func)
  {
    auto& delegate = mInputSystems.emplace_back();
    delegate = std::move(func);
  }

  template <auto Func>
  void add_render_system()
  {
    auto& delegate = mRenderSystems.emplace_back();
    delegate.template connect<Func>();
  }

  template <auto Func, typename T>
  void add_render_system(T* self)
  {
    auto& delegate = mRenderSystems.emplace_back();
    delegate.template connect<Func>(self);
  }

  template <std::invocable<const entt::registry&> T>
  void add_render_system(T func)
  {
    auto& delegate = mRenderSystems.emplace_back();
    delegate = std::move(func);
  }

  [[nodiscard]] auto registry() noexcept -> entt::registry&
  {
    return mRegistry;
  }

  [[nodiscard]] auto registry() const noexcept -> const entt::registry&
  {
    return mRegistry;
  }

  [[nodiscard]] auto dispatcher() noexcept -> entt::dispatcher&
  {
    return mDispatcher;
  }

  [[nodiscard]] auto dispatcher() const noexcept -> const entt::dispatcher&
  {
    return mDispatcher;
  }

  [[nodiscard]] auto keyboard() const noexcept -> const cen::keyboard&
  {
    return mKeyboard;
  }

  [[nodiscard]] auto mouse() const noexcept -> const cen::mouse&
  {
    return mMouse;
  }

  [[nodiscard]] auto window() noexcept -> cen::window&
  {
    return mWindow;
  }

  [[nodiscard]] auto window() const noexcept -> const cen::window&
  {
    return mWindow;
  }

 private:
  using input_func = entt::delegate<void(entt::registry&, entt::dispatcher&)>;
  using logic_func = entt::delegate<void(entt::registry&, entt::dispatcher&, delta_type)>;
  using render_func = entt::delegate<void(entt::registry&)>;

  cen::window mWindow;
  cen::keyboard mKeyboard;
  cen::mouse mMouse;

  seconds_type::rep mRate;
  int mMaxFramesPerTick;
  seconds_type mDelta;
  seconds_type mCurrent;
  bool mRunning{true};

  entt::registry mRegistry;
  entt::dispatcher mDispatcher;

  entt::delegate<void()> mOnStart;
  entt::delegate<void()> mOnExit;

  std::vector<input_func> mInputSystems;
  std::vector<logic_func> mLogicSystems;
  std::vector<render_func> mRenderSystems;

  void tick()
  {
    const auto newTime = cen::counter::now_in_seconds<seconds_type::rep>();
    auto frameTime = newTime - mCurrent;

    mCurrent = newTime;
    auto nSteps = 0;

    while (frameTime > seconds_type::zero())
    {
      if (nSteps > mMaxFramesPerTick)
      {
        break;  // avoids spiral-of-death by limiting maximum amount of steps
      }

      mRunning = update_input();
      if (!mRunning)
      {
        break;
      }

      const auto dt = std::min(frameTime, mDelta);
      update_logic(static_cast<delta_type>(dt.count()));

      frameTime -= dt;

      ++nSteps;
    }
  }

  auto update_input() -> bool
  {
    if (auto renderer = cen::get_renderer(mWindow))
    {
      mMouse.update(renderer.output_size());
    }
    else
    {
      mMouse.update();
    }

    mKeyboard.update();
    cen::event::update();

    for (const auto& system : mInputSystems)
    {
      system(mRegistry, mDispatcher);
    }

    return !cen::event::in_queue(cen::event_type::quit);
  }

  void update_logic(const delta_type dt)
  {
    mDispatcher.update();
    for (const auto& system : mLogicSystems)
    {
      system(mRegistry, mDispatcher, dt);
    }
  }

  void render()
  {
    for (const auto& system : mRenderSystems)
    {
      system(mRegistry);
    }
  }
};

/// \} End of group core

}  // namespace rune

#endif  // RUNE_ENGINE_HPP_

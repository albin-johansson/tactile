#ifndef RUNE_CORE_ENGINE_HPP
#define RUNE_CORE_ENGINE_HPP

#include <cassert>        // assert
#include <centurion.hpp>  // window, renderer, ...
#include <concepts>       // derived_from, constructible_from

#include "../aliases/maybe.hpp"
#include "game.hpp"
#include "graphics.hpp"
#include "input.hpp"
#include "semi_fixed_game_loop.hpp"

namespace rune {

/// \addtogroup core
/// \{

// clang-format off

template <typename T>
concept is_configuration_type = requires
{
  { T::renderer_flags } -> std::convertible_to<uint32>;
  { T::window_size } -> std::convertible_to<cen::iarea>;
};

// clang-format on

/**
 * \struct configuration
 *
 * \brief Provides configuration options for different engine aspects.
 *
 * \note Members are initialized to their default values, meaning that you do not have to
 * assign each member if you create custom configurations.
 *
 * \see `engine`
 */
struct configuration
{
  uint32 renderer_flags = cen::renderer::default_flags();
  cen::iarea window_size = cen::window::default_size();
};

static_assert(is_configuration_type<configuration>);

/**
 * \class engine
 *
 * \brief Represents the core engine in the framework.
 *
 * \details The easiest way to set up your game is to use either of
 * `RUNE_IMPLEMENT_MAIN_WITH_GAME` or `RUNE_IMPLEMENT_MAIN_WITH_ENGINE` to automatically
 * generate a correct definition of the `main` function.
 *
 * \details Alternatively, you could manually initialize an `engine` instance with your
 * game and graphics types and call the `engine::run()` function to start your game.
 * However, remember to initialize Centurion _before_ creating your engine instance!
 *
 * \details It is perfectly valid to derive from this class, which enables easy access to
 * the game window, graphics context, input state, etc.
 *
 * \details The game class must either be default-constructible or provide a constructor
 * that accepts `graphics_type&`.
 *
 * \tparam Game the type of the game class.
 * \tparam Graphics the type of the graphics context.
 *
 * \see `game_base`
 * \see `graphics`
 * \see `RUNE_IMPLEMENT_MAIN_WITH_ENGINE`
 * \see `RUNE_IMPLEMENT_MAIN_WITH_GAME`
 * \see `RUNE_IMPLEMENT_MAIN_WITH_GAME_AND_GRAPHICS`
 *
 * \since 0.1.0
 */
template <typename Game, typename Graphics = graphics>
class engine
{
  // To be able to access update_logic and update_input
  friend class semi_fixed_game_loop<Game, Graphics>;

 public:
  using game_type = Game;          ///< Game class type.
  using graphics_type = Graphics;  ///< Graphics context type.
  using loop_type = semi_fixed_game_loop<game_type, graphics_type>;  ///< Game loop type.

  static_assert(std::constructible_from<game_type, graphics_type&> ||
                    std::default_initializable<game_type>,
                "Game class must either be default constructible or provide a "
                "constructor that accepts \"graphics_type&\"");

  /**
   * \brief Creates an engine instance.
   *
   * \param cfg optional custom configuration of the engine.
   */
  explicit engine(const configuration& cfg = default_cfg())
      : m_loop{this}
      , m_window{"Rune", cfg.window_size}
      , m_graphics{m_window, cfg.renderer_flags}
  {
    if constexpr (std::constructible_from<game_type, graphics_type&>)
    {
      m_game.emplace(m_graphics);
    }
    else
    {
      m_game.emplace();
    }

    m_game->init(m_graphics);
  }

  /**
   * \brief Starts the game loop and runs the game.
   *
   * \return always `0`.
   */
  auto run() -> int
  {
    assert(m_game);

    m_window.show();
    m_loop.fetch_current_time();

    m_game->on_start();

    auto& renderer = m_graphics.get_renderer();
    while (m_loop.is_running())
    {
      m_loop.tick();
      m_game->render(m_graphics);
    }

    m_game->on_exit();
    m_window.hide();

    return 0;
  }

  /**
   * \brief Returns the associated game window.
   *
   * \return the associated window.
   */
  [[nodiscard]] auto get_window() noexcept -> cen::window&
  {
    return m_window;
  }

  /// \copydoc get_window()
  [[nodiscard]] auto get_window() const noexcept -> const cen::window&
  {
    return m_window;
  }

  /**
   * \brief Returns the associated game instance.
   *
   * \return the game instance.
   */
  [[nodiscard]] auto get_game() -> game_type&
  {
    assert(m_game);
    return *m_game;
  }

  /// \copydoc get_game()
  [[nodiscard]] auto get_game() const -> const game_type&
  {
    assert(m_game);
    return *m_game;
  }

  /**
   * \brief Returns the associated graphics context.
   *
   * \return the graphics context.
   */
  [[nodiscard]] auto get_graphics() noexcept -> graphics_type&
  {
    return m_graphics;
  }

  /// \copydoc get_graphics()
  [[nodiscard]] auto get_graphics() const noexcept -> const graphics_type&
  {
    return m_graphics;
  }

  /**
   * \brief Returns the current input state.
   *
   * \return the input state.
   */
  [[nodiscard]] auto get_input() noexcept -> input&
  {
    return m_input;
  }

  /// \copydoc get_input()
  [[nodiscard]] auto get_input() const noexcept -> const input&
  {
    return m_input;
  }

  /**
   * \brief Returns the default configuration used by the engine, if no custom
   * configuration is requested.
   *
   * \return the default engine configuration.
   */
  [[nodiscard]] constexpr static auto default_cfg() -> configuration
  {
    return configuration{};
  }

 private:
  loop_type m_loop;          ///< The game loop.
  cen::window m_window;      ///< The associated window.
  graphics_type m_graphics;  ///< The graphics context.
  input m_input;             ///< The input state wrapper.
  maybe<game_type> m_game;   ///< The game instance, optional to delay construction.

  void update_logic(const float dt)
  {
    m_game->tick(dt);
  }

  auto update_input() -> bool
  {
    const auto& renderer = m_graphics.get_renderer();
    m_input.mouse.set_logical_size(renderer.logical_size());
    m_input.mouse.update(renderer.output_size());
    m_input.keyboard.update();

    cen::event::update();

    m_game->handle_input(m_input);

    return !m_game->should_quit() && !cen::event::in_queue(cen::event_type::quit);
  }
};

#define RUNE_IMPLEMENT_MAIN_WITH_ENGINE(Engine) \
  int main(int, char**)                         \
  {                                             \
    cen::library centurion;                     \
    Engine engine;                              \
    return engine.run();                        \
  }

#define RUNE_IMPLEMENT_MAIN_WITH_GAME(Game)    \
  int main(int, char**)                        \
  {                                            \
    cen::library centurion;                    \
    rune::engine<Game, rune::graphics> engine; \
    return engine.run();                       \
  }

#define RUNE_IMPLEMENT_MAIN_WITH_GAME_AND_GRAPHICS(Game, Graphics) \
  int main(int, char**)                                            \
  {                                                                \
    cen::library centurion;                                        \
    rune::engine<Game, Graphics> engine;                           \
    return engine.run();                                           \
  }

/// \} End of group core

}  // namespace rune

#endif  // RUNE_CORE_ENGINE_HPP

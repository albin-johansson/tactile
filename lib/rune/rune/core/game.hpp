#ifndef RUNE_CORE_GAME_HPP
#define RUNE_CORE_GAME_HPP

#include <concepts>  // derived_from

#include "core/graphics.hpp"
#include "core/input.hpp"

namespace rune {

/// \addtogroup core
/// \{

/**
 * \class game_base
 *
 * \brief The base class for all game class implementations.
 *
 * \details You should derive your game class from this class, which you then supply to
 * your engine instance.
 *
 * \details The following example illustrates a skeleton for a game using the Rune
 * framework. Note, no virtual functions are pure, i.e. you only have to override the
 * functions that you need in your own game class.
 *
 * \note Whilst this class is stateless and default-constructible, the `engine` class
 * expects game classes to be either default-constructible or constructible from a
 * reference to a graphics context.
 *
 * \code{cpp}
 * #include <rune.hpp>
 *
 * class AwesomeGame final : public rune::game_base
 * {
 *  public:
 *   explicit AwesomeGame(graphics_type& graphics)
 *   {
 *     // ...
 *   }
 *
 *   void handle_input(const rune::input& input) override
 *   {
 *     // ...
 *   }
 *
 *   void tick(float dt) override
 *   {
 *     // ...
 *   }
 *
 *   void render(graphics_type& graphics) const override
 *   {
 *     // ...
 *   }
 *
 *   [[nodiscard]] bool should_quit() const override
 *   {
 *     // ...
 *   }
 * };
 *
 * RUNE_IMPLEMENT_MAIN_WITH_GAME(AwesomeGame)
 * \endcode
 *
 * \see `game_base`
 * \see `engine`
 * \see `graphics`
 * \see `input`
 * \see `RUNE_IMPLEMENT_MAIN_WITH_GAME`
 * \see `RUNE_IMPLEMENT_MAIN_WITH_ENGINE`
 *
 * \since 0.1.0
 */
template <std::derived_from<graphics> Graphics>
class basic_game
{
 public:
  using graphics_type = Graphics;

  virtual ~basic_game() noexcept = default;

  /**
   * \brief Initializes the game state.
   *
   * \details This function is called just after the game class is constructed. A use case
   * for this function could be if you do not want to provide a constructor in your game
   * class that accepts a graphics context by reference.
   *
   * \param gfx the associated graphics context.
   *
   * \since 0.1.0
   */
  virtual void init(graphics_type& gfx)
  {}

  /**
   * \brief Invoked just before the game starts running.
   *
   * \details This function is guaranteed to only be called once just before the game loop
   * starts running.
   *
   * \since 0.1.0
   */
  virtual void on_start()
  {}

  /**
   * \brief Invoked just before the game stops running.
   *
   * \details This function is guaranteed to only be called once just after the game loop
   * has finished running, i.e. just before the game shuts down.
   *
   * \since 0.1.0
   */
  virtual void on_exit()
  {}

  /**
   * \brief Handles the current input state.
   *
   * \details This function is called each frame and is meant to handle user input.
   *
   * \details This function is called each frame _before_ both `tick()` and `render()`.
   *
   * \param input the current input state.
   *
   * \since 0.1.0
   */
  virtual void handle_input(const input& input)
  {}

  /**
   * \brief Updates the state of the game by one frame according to the delta.
   *
   * \details This function updates the core logic of the game.
   *
   * \details This function is called each frame _after_ `handle_input()` and _before_
   * `render()`.
   *
   * \param dt the current delta time, in seconds.
   *
   * \since 0.1.0
   */
  virtual void tick(float dt)
  {}

  /**
   * \brief Renders the current game state.
   *
   * \details This function is called each frame _after_ both `handle_input()` and
   * `tick()`.
   *
   * \details If you've specified a custom graphics context type (that must derive from
   * `graphics`), then you can safely downcast the supplied graphics context using
   * `dynamic_cast` (or even `static_cast`, the engine is guaranteed to use your graphics
   * context type if specified).
   *
   * \note Remember to clear the rendering target and present your draw commands at the
   * beginning and end of your rendering function, respectively.
   *
   * \param gfx the graphics context used for rendering.
   *
   * \since 0.1.0
   */
  virtual void render(graphics_type& gfx) const
  {}

  /**
   * \brief Indicates whether or not the game should stop running.
   *
   * \details By default, this function returns `true`.
   *
   * \return `true` if the game should stop running; `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] virtual auto should_quit() const -> bool
  {
    return false;
  }
};

using game_base = basic_game<graphics>;

/// \} End of group core

}  // namespace rune

#endif  // RUNE_CORE_GAME_HPP
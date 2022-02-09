#pragma once

#include <array>        // array
#include <string>       // string
#include <string_view>  // string_view

#include <entt/entt.hpp>

#include "dialog.hpp"
#include "tactile.hpp"

namespace tactile {

/**
 * \brief An abstract class representing dialogs which provide a string input field.
 */
class string_input_dialog : public dialog_base
{
 public:
  explicit string_input_dialog(const char* title);

  void show(std::string previous);

 protected:
  /**
   * \brief Sets the input hint used by the string field.
   *
   * \param hint optional hint label; a null pointer disables the input hint.
   */
  void set_input_hint(const char* hint);

  void on_update(const document_model& model, entt::dispatcher& dispatcher) final;

  [[nodiscard]] auto is_current_input_valid(const document_model& model) const
      -> bool final;

  /**
   * \brief Validates an input string.
   *
   * \param model the associated model instance.
   * \param input the current input string.
   *
   * \return `true` if the input string is valid; `false` otherwise.
   */
  [[nodiscard]] virtual auto validate(const document_model& model,
                                      std::string_view input) const -> bool = 0;

  /**
   * \brief Returns the current string input.
   *
   * \return a view into the current input.
   */
  [[nodiscard]] auto current_input() const -> std::string_view;

  /**
   * \brief Returns the initial input string.
   *
   * \return the initial input.
   */
  [[nodiscard]] auto previous_input() const -> const std::string&;

 private:
  const char* mHint{};
  std::string mPrevious;
  std::array<char, 128> mBuffer{};
  bool mShouldAcquireFocus{};
};

}  // namespace tactile
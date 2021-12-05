#pragma once

#include <string>       // string
#include <string_view>  // string_view
#include <array>        // array

#include <tactile_def.hpp>

#include <entt/entt.hpp>

namespace Tactile {

class ARenameDialog {
 public:
  explicit ARenameDialog(CStr title);

  virtual ~ARenameDialog() = default;

  void Update(const entt::registry& registry, entt::dispatcher& dispatcher);

  void Show(std::string oldName);

 protected:
  virtual void OnAccept(entt::dispatcher& dispatcher, const std::string& input) = 0;

  [[nodiscard]] virtual auto IsInputValid(const entt::registry& registry,
                                          std::string_view input) -> bool = 0;

  [[nodiscard]] auto GetPreviousName() const -> const std::string&;

 private:
  CStr mTitle{};
  std::string mOldName;
  std::array<char, 128> mNameBuffer{};
  bool mIsInputValid{};
  bool mShow{};
};

}  // namespace Tactile
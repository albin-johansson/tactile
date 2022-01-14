#pragma once

#include <utility>  // forward

#include <imgui.h>
#include <tactile_def.hpp>

namespace Tactile::Scoped {

struct ID final
{
  explicit ID(const void* ptr) { ImGui::PushID(ptr); }

  explicit ID(const CStr str) { ImGui::PushID(str); }

  explicit ID(const int id) { ImGui::PushID(id); }

  ~ID() { ImGui::PopID(); }
};

struct Disable final
{
  explicit Disable(const bool disable = true) { ImGui::BeginDisabled(disable); }

  ~Disable() { ImGui::EndDisabled(); }
};

struct Tooltip final
{
  Tooltip() { ImGui::BeginTooltip(); }

  ~Tooltip() { ImGui::EndTooltip(); }
};

class StyleVar final {
 public:
  StyleVar(const ImGuiStyleVar index, const ImVec2& value)
  {
    ImGui::PushStyleVar(index, value);
  }

  StyleVar(const ImGuiStyleVar index, const float value)
  {
    ImGui::PushStyleVar(index, value);
  }

  ~StyleVar() { Pop(); }

  void Pop()
  {
    if (!mPopped) {
      ImGui::PopStyleVar();
      mPopped = true;
    }
  }

 private:
  bool mPopped{};
};

class StyleColor final {
 public:
  StyleColor(const ImGuiCol index, const ImVec4& color)
  {
    ImGui::PushStyleColor(index, color);
  }

  StyleColor(const ImGuiCol index, const uint32 value)
  {
    ImGui::PushStyleColor(index, value);
  }

  ~StyleColor() { Pop(); }

  void Pop()
  {
    if (!mPopped) {
      ImGui::PopStyleColor();
      mPopped = true;
    }
  }

 private:
  bool mPopped{};
};

class Group final {
 public:
  Group() { ImGui::BeginGroup(); }

  ~Group() { ImGui::EndGroup(); }
};

class Child final {
 public:
  explicit Child(const CStr id,
                 const ImVec2& size = {0, 0},
                 const bool border = false,
                 const ImGuiWindowFlags flags = 0)
      : mOpen{ImGui::BeginChild(id, size, border, flags)}
  {}

  ~Child() { ImGui::EndChild(); }

  [[nodiscard]] auto IsOpen() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

class Combo final {
 public:
  Combo(const CStr name, const CStr current) : mOpen{ImGui::BeginCombo(name, current)} {}

  ~Combo()
  {
    if (mOpen) {
      ImGui::EndCombo();
    }
  }

  [[nodiscard]] auto IsOpen() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

class TabBar final {
 public:
  explicit TabBar(const CStr name, const ImGuiTabBarFlags flags = 0)
      : mOpen{ImGui::BeginTabBar(name, flags)}
  {}

  ~TabBar()
  {
    if (mOpen) {
      ImGui::EndTabBar();
    }
  }

  [[nodiscard]] auto IsOpen() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

class TabItem final {
 public:
  explicit TabItem(const CStr name,
                   bool* open = nullptr,
                   const ImGuiTabItemFlags flags = 0)
      : mOpen{ImGui::BeginTabItem(name, open, flags)}
  {}

  ~TabItem()
  {
    if (mOpen) {
      ImGui::EndTabItem();
    }
  }

  [[nodiscard]] auto IsOpen() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

class Table final {
 public:
  Table(const CStr name, const int nColumns, const ImGuiTableFlags flags = 0)
      : mOpen{ImGui::BeginTable(name, nColumns, flags)}
  {}

  ~Table()
  {
    if (mOpen) {
      ImGui::EndTable();
    }
  }

  [[nodiscard]] auto IsOpen() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

class Popup final {
 public:
  explicit Popup(const CStr name, const ImGuiWindowFlags flags = 0)
      : mOpen{ImGui::BeginPopup(name, flags)}
  {}

  [[nodiscard]] static auto ForItem(const CStr name, const ImGuiPopupFlags flags = 1)
      -> Popup
  {
    return Popup{ImGui::BeginPopupContextItem(name, flags)};
  }

  [[nodiscard]] static auto ForWindow(const CStr name, const ImGuiPopupFlags flags = 1)
      -> Popup
  {
    return Popup{ImGui::BeginPopupContextWindow(name, flags)};
  }

  ~Popup()
  {
    if (mOpen) {
      ImGui::EndPopup();
    }
  }

  [[nodiscard]] auto IsOpen() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};

  explicit Popup(const bool open) : mOpen{open} {}
};

class ListBox final {
 public:
  explicit ListBox(const CStr label, const ImVec2& size = {0, 0})
      : mOpen{ImGui::BeginListBox(label, size)}
  {}

  ~ListBox()
  {
    if (mOpen) {
      ImGui::EndListBox();
    }
  }

  [[nodiscard]] auto IsOpen() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

class Menu final {
 public:
  explicit Menu(const CStr name, const bool enabled = true)
      : mOpen{ImGui::BeginMenu(name, enabled)}
  {}

  ~Menu()
  {
    if (mOpen) {
      ImGui::EndMenu();
    }
  }

  [[nodiscard]] auto IsOpen() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

class Modal final {
 public:
  explicit Modal(const CStr name, const ImGuiWindowFlags flags = 0, bool* open = nullptr)
      : mOpen{ImGui::BeginPopupModal(name, open, flags)}
  {}

  ~Modal()
  {
    if (mOpen) {
      ImGui::EndPopup();
    }
  }

  [[nodiscard]] auto IsOpen() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

class Window final {
 public:
  explicit Window(const CStr label,
                  const ImGuiWindowFlags flags = 0,
                  bool* open = nullptr)
      : mOpen{ImGui::Begin(label, open, flags)}
  {}

  ~Window() { ImGui::End(); }

  [[nodiscard]] auto IsFocused(const ImGuiFocusedFlags flags = 0) const -> bool
  {
    return mOpen && ImGui::IsWindowFocused(flags);
  }

  [[nodiscard]] auto IsOpen() const noexcept -> bool { return mOpen; }

  [[nodiscard]] static auto CurrentWindowContainsMouse() -> bool
  {
    const auto min = ImGui::GetWindowContentRegionMin();
    const auto max = ImGui::GetWindowContentRegionMax();
    return ImGui::IsMouseHoveringRect(min, max);
  }

 private:
  bool mOpen{};
};

class TreeNode final {
 public:
  explicit TreeNode(const CStr id, const ImGuiTreeNodeFlags flags = 0)
      : mOpen{ImGui::TreeNodeEx(id, flags)}
  {}

  template <typename... Args>
  explicit TreeNode(const CStr id,
                    const ImGuiTreeNodeFlags flags,
                    const CStr fmt,
                    Args&&... args)
      : mOpen{ImGui::TreeNodeEx(id, flags, fmt, std::forward<Args>(args)...)}
  {}

  ~TreeNode()
  {
    if (mOpen) {
      ImGui::TreePop();
    }
  }

  [[nodiscard]] auto IsOpen() const noexcept -> bool { return mOpen; }

 private:
  bool mOpen{};
};

}  // namespace Tactile::Scoped
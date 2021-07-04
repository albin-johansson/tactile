#include "add_property_dialog.hpp"

#include <imgui.h>

#include <array>  // array
#include <centurion.hpp>

#include "core/document.hpp"
#include "core/events/properties/add_property_event.hpp"
#include "core/tactile_error.hpp"
#include "gui/widgets/button_ex.hpp"
#include "utils/buffer_utils.hpp"

namespace Tactile {
namespace {

inline constexpr auto type_options =
    "string\0int\0float\0bool\0color\0object\0file\0\0";

inline int type_index = 0;
inline std::array<char, 100> name_buffer{};

inline bool is_input_valid = false;

void ResetState()
{
  type_index = 0;
  ZeroBuffer(name_buffer);
  is_input_valid = false;
}

[[nodiscard]] auto GetSelectedType() -> PropertyType
{
  switch (type_index)
  {
    case 0:
      return PropertyType::String;

    case 1:
      return PropertyType::Integer;

    case 2:
      return PropertyType::Floating;

    case 3:
      return PropertyType::Boolean;

    case 4:
      return PropertyType::Color;

    case 5:
      return PropertyType::Object;

    case 6:
      return PropertyType::File;

    default:
      throw TactileError{"Invalid property type index!"};
  }
}

}  // namespace

void UpdateAddPropertyDialog(const IDocument& document,
                             entt::dispatcher& dispatcher)
{
  constexpr auto flags =
      ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;
  if (ImGui::BeginPopupModal("Add property", nullptr, flags))
  {
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Name: ");

    ImGui::SameLine();
    if (ImGui::InputText("##NameInput", name_buffer.data(), sizeof name_buffer))
    {
      const auto name = CreateStringFromBuffer(name_buffer);
      const auto& properties = document.GetProperties();
      is_input_valid = !name.empty() && !properties.contains(name);
    }

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Type: ");

    ImGui::SameLine();
    ImGui::Combo("##TypeCombo", &type_index, type_options);

    ImGui::Spacing();
    if (ButtonEx("OK", nullptr, is_input_valid))
    {
      dispatcher.enqueue<AddPropertyEvent>(CreateStringFromBuffer(name_buffer),
                                           GetSelectedType());
      ResetState();
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Cancel"))
    {
      ResetState();
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
}

void OpenAddPropertyDialog()
{
  ImGui::OpenPopup("Add property");
}

}  // namespace Tactile

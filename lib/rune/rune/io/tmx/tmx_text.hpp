#ifndef RUNE_TMX_TEXT_HPP
#define RUNE_TMX_TEXT_HPP

#include <string>  // string

#include "../../aliases/json_type.hpp"
#include "../json_utils.hpp"
#include "tmx_color.hpp"
#include "tmx_halign.hpp"
#include "tmx_valign.hpp"

namespace rune {

/// \addtogroup tmx
/// \{

struct tmx_text final
{
  std::string text;
  std::string family{"sans-serif"};
  tmx_color color;
  tmx_halign horizontal_alignment{tmx_halign::left};
  tmx_valign vertical_alignment{tmx_valign::top};
  int pixel_size{16};
  bool bold{};
  bool italic{};
  bool kerning{true};
  bool strikeout{};
  bool underline{};
  bool wrap{};
};

inline void from_json(const json_type& json, tmx_text& text)
{
  json.at("text").get_to(text.text);

  io::try_get_to(json, "fontfamily", text.family);
  io::try_get_to(json, "halign", text.horizontal_alignment);
  io::try_get_to(json, "valign", text.vertical_alignment);
  io::try_get_to(json, "pixelsize", text.pixel_size);
  io::try_get_to(json, "bold", text.bold);
  io::try_get_to(json, "italic", text.italic);
  io::try_get_to(json, "kerning", text.kerning);
  io::try_get_to(json, "strikeout", text.strikeout);
  io::try_get_to(json, "underline", text.underline);
  io::try_get_to(json, "wrap", text.wrap);

  if (const auto it = json.find("color"); it != json.end())
  {
    text.color = tmx::make_color(it->get<std::string>());
  }
}

/// \} End of group tmx

}  // namespace rune

#endif  // RUNE_TMX_TEXT_HPP

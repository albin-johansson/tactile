#ifndef RUNE_TMX_OBJECT_HPP
#define RUNE_TMX_OBJECT_HPP

#include <memory>   // unique_ptr, make_unique
#include <string>   // string
#include <variant>  // variant, monostate
#include <vector>   // vector

#include "../../aliases/integers.hpp"
#include "../../aliases/json_type.hpp"
#include "../../aliases/maybe.hpp"
#include "../json_utils.hpp"
#include "tmx_global_id.hpp"
#include "tmx_point.hpp"
#include "tmx_property.hpp"
#include "tmx_text.hpp"

namespace rune {

/// \addtogroup tmx
/// \{

struct tmx_object;

struct tmx_polygon final
{
  std::vector<tmx_point> points;
};

struct tmx_polyline final
{
  std::vector<tmx_point> points;
};

struct tmx_template_object final
{
  std::string template_file;
  std::unique_ptr<tmx_object> object;
  maybe<std::string> tileset_source;
  maybe<tmx_global_id> tileset_first_id;
};

struct tmx_object final
{
  using data_type = std::variant<std::monostate,
                                 tmx_polygon,
                                 tmx_polyline,
                                 tmx_text,
                                 tmx_template_object,
                                 tmx_global_id>;

  int id{};
  float x{};
  float y{};
  float width{};
  float height{};
  float rotation{};
  std::string name;
  std::string type;
  tmx_properties properties;
  data_type data;
  bool is_ellipse{};
  bool is_point{};
  bool visible{true};
};

inline void from_json(const json_type& json, tmx_polygon& polygon)
{
  io::get_to(json, polygon.points);
}

inline void from_json(const json_type& json, tmx_polyline& line)
{
  io::get_to(json, line.points);
}

void from_json(const json_type& json, tmx_object& object);

inline void from_json(const json_type& json, tmx_template_object& object)
{
  json.at("template").get_to(object.template_file);
  object.object = std::make_unique<tmx_object>(json.at("object").get<tmx_object>());

  if (const auto it = json.find("tileset"); it != json.end())
  {
    io::emplace_to(json, "firstgid", object.tileset_first_id);
    object.tileset_source = json.at("source").get<std::string>();
  }
}

inline void from_json(const json_type& json, tmx_object& object)
{
  json.at("id").get_to(object.id);
  json.at("x").get_to(object.x);
  json.at("y").get_to(object.y);
  json.at("width").get_to(object.width);
  json.at("height").get_to(object.height);
  json.at("rotation").get_to(object.rotation);
  json.at("name").get_to(object.name);
  json.at("type").get_to(object.type);
  json.at("visible").get_to(object.visible);

  io::try_get_to(json, "ellipse", object.is_ellipse);
  io::try_get_to(json, "point", object.is_point);

  io::try_get_to(json, "properties", object.properties);

  if (const auto it = json.find("gid"); it != json.end())
  {
    object.data.emplace<tmx_global_id>(it->get<tmx_global_id::value_type>());
  }

  io::try_emplace_to<tmx_text>(json, "text", object.data);
  io::try_emplace_to<tmx_polygon>(json, "polygon", object.data);
  io::try_emplace_to<tmx_polyline>(json, "polyline", object.data);
  io::try_emplace_to<tmx_template_object>(json, "template", object.data);
}

/// \} End of group tmx

}  // namespace rune

#endif  // RUNE_TMX_OBJECT_HPP

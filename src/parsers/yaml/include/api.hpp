#pragma once

#ifdef WIN32
#ifdef TACTILE_YAML_PARSER_EXPORT
#define TACTILE_YAML_PARSER_API __declspec(dllexport)
#else
#define TACTILE_YAML_PARSER_API __declspec(dllimport)
#endif  // TACTILE_YAML_PARSER_EXPORT
#else
#define TACTILE_YAML_PARSER_API
#endif  // WIN32

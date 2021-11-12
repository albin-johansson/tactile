#pragma once

#include <fstream>  // ofstream
#include <ios>      // ios

#include <tactile_def.hpp>

#include "build.hpp"

#if TACTILE_COMPILER_MSVC
#define TACTILE_IO_PATH_STR(str) L##str
#else
#define TACTILE_IO_PATH_STR(str) str
#endif  // TACTILE_COMPILER_MSVC

namespace Tactile {

inline void WriteToFile(const CStr file, const CStr data)
{
  std::ofstream stream{file, std::ios::out | std::ios::trunc};
  stream << data;
}

}  // namespace Tactile
